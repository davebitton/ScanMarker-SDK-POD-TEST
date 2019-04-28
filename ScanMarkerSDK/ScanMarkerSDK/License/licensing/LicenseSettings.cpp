#include "stdafx.h"
#include "LicenseSettings.h"
#include <fstream>
#include "cJSON.h"

#include <memory>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#ifdef __ANDROID__
#include <time64.h>
#endif

#include "licensing.h"
#include "LicensingIO.h"
#include "LicensingLog.h"
#include "LicensingCrypto.h"
#include "LicensingMemory.h"

LicenseSettings *LicenseSettings::m_pInstance = 0;

LicenseSettings::LicenseSettings()
#ifdef __ANDROID__
: m_env(0)
, m_initialized (false)
#endif
{
}


LicenseSettings * LicenseSettings::instance()
{
	if (m_pInstance == 0) 
	{
		m_pInstance = new LicenseSettings();
	}
	return m_pInstance;
}
#ifdef __ANDROID__
void LicenseSettings::initSettings(JNIEnv *env, const char *activationUrl
                                   , const char *deactivationUrl, const char *machineUniqueId
                                   , const char *scannerId, const char *licensePath)
#elif(defined LICENSE_IOS)
void LicenseSettings::initSettings(const char *activationUrl, const char *deactivationUrl, const char *machineUniqueId,  const char *scannerId, const char *licensePath)
#else
void LicenseSettings::initSettings(const char *activationUrl, const char *deactivationUrl)
#endif
{
	LicenseSettings *pInstance = instance();
	pInstance->m_activationUrl = activationUrl;
	pInstance->m_deactivationUrl = deactivationUrl;
    
#ifdef __ANDROID__
    pInstance->m_env = env;
    pInstance->m_scannerId = scannerId;
    //to spare redundant license file search, from now on we adopt (at least on Android) the following convention for
    //the license file name:
    //license<scanner_serial>.json
    pInstance->m_licenseFileName = licensePath;
    pInstance->m_licenseFileName.append("/");
    pInstance->m_licenseFileName.append(scannerId);
    pInstance->m_licenseFileName.append(".json");
    pInstance->m_machineUniqueId = machineUniqueId;
    
    std::ostringstream logMsgStream;
    logMsgStream << "LicenseSettings::initSettings: init settings with scannerId=";
    logMsgStream << scannerId << ", licensePath=" << licensePath;
    log(Warn, logMsgStream.str().c_str());
    
#elif (defined LICENSE_IOS)
    pInstance->m_scannerId = scannerId;
    //to spare redundant license file search, from now on we adopt (at least on Android) the following convention for
    //the license file name:
    //license<scanner_serial>.json
    pInstance->m_licenseFileName = licensePath;
    pInstance->m_licenseFileName.append("/");
    pInstance->m_licenseFileName.append(scannerId);
    pInstance->m_licenseFileName.append(".json");
    pInstance->m_machineUniqueId = machineUniqueId;
    
    std::ostringstream logMsgStream;
    logMsgStream << "LicenseSettings::initSettings: init settings with scannerId=";
    logMsgStream << scannerId << ", licensePath=" << licensePath;
    log(Warn, logMsgStream.str().c_str());
#endif //__ANDROID__
#if(USE_MACHINE_ID_CODE !=1)//mobile
	pInstance->m_machineUniqueId = machineUniqueId;

#endif
    pInstance->m_initialized = true;
}

#ifdef __ANDROID__

std::string LicenseSettings::getLicenseFileName() const
{
    return m_licenseFileName;
}

int LicenseSettings::sendRequest(const std::string &urlIn, const std::string &jsonIn, std::string &jsonOut)
{
    jint i;

    jclass cls = m_env->FindClass("com/scanmarker/license/client/WebRequestHandler");
    jmethodID constructor = m_env->GetMethodID(cls, "<init>", "()V");
    jobject object = m_env->NewObject(cls, constructor);
    jmethodID methodId = m_env->GetMethodID(cls, "sendRequest", "(Ljava/lang/String;Ljava/lang/String;)I");

    jstring activationUrlJava = m_env->NewStringUTF(urlIn.c_str());
    jstring jsonInJava = m_env->NewStringUTF(jsonIn.c_str());

    jint paramInt = (jint) m_env->CallIntMethod(object, methodId, activationUrlJava, jsonInJava);
    int rv = (int)paramInt;
    if (!rv)
    {
        methodId = m_env->GetMethodID(cls, "getResponse", "()Ljava/lang/String;");
        jstring stringRv = (jstring) (m_env->CallObjectMethod(object, methodId));
        const char *strJsonResponse = m_env->GetStringUTFChars(stringRv, 0);
        if (strJsonResponse){
            jsonOut = strJsonResponse;
            m_env->ReleaseStringUTFChars(stringRv, strJsonResponse);
        }
    }
    m_env->DeleteLocalRef(object);
    return rv;
}

int LicenseSettings::checkSignature(const std::string &signature, const char *msg, int msgLen, const std::string &encodedCert)
{
	jclass cls = m_env->FindClass("com/scanmarker/license/client/CryptoHandler");
    jmethodID staticMethodId = m_env->GetStaticMethodID(cls, "checkSignature", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");

    if (staticMethodId == 0)
        return -1;

    jstring signatureInJava = m_env->NewStringUTF(signature.c_str());
    jstring msgInJava = m_env->NewStringUTF(msg);
    jstring certInJava = m_env->NewStringUTF(encodedCert.c_str());
    return m_env->CallStaticIntMethod(cls, staticMethodId, signatureInJava, msgInJava, certInJava);
}

#endif //__ANDROID__

LicenseSettings::~LicenseSettings()
{
}

std::string LicenseSettings::getActivationUrl() const
{
	return m_activationUrl;
}

std::string LicenseSettings::getDeactivationUrl() const
{
	return m_deactivationUrl;
}


static const std::string s_notSet = "NOT_SET";

LicenseData::LicenseData()
: m_valid(true)
, m_deviceId(s_notSet)
, m_licenseType(TEMPORARY)
, m_ownerName(s_notSet)
, m_email(s_notSet)
, m_phoneNumber(s_notSet)
, m_licenseJsonFile(s_notSet)
, m_machineUniqueId(s_notSet)
, m_licenseNumber(s_notSet)
, m_signature(s_notSet)
, m_expirationDateString(s_notSet)
, m_certEncoded(s_notSet)
, m_lastError(Success)
{
}

LicenseData::LicenseData(LicenseType licenseType, const std::string &deviceId, const std::string &licenseJsonFile)
	: m_licenseType(licenseType)
	, m_deviceId(deviceId)
	, m_licenseJsonFile(licenseJsonFile)
	, m_valid(true)
	, m_ownerName(s_notSet)
	, m_email(s_notSet)
	, m_phoneNumber(s_notSet)
	, m_machineUniqueId(s_notSet)
	, m_licenseNumber(s_notSet)
	, m_signature(s_notSet)
	, m_expirationDateString(s_notSet)
	, m_certEncoded(s_notSet)
    , m_lastError(Success)
{
}

//never use it to generate a license on the client side, because there could be spaces etc, which will disrupt the signature check
//for instance, product_id is missing in LicenseData
LicenseData & LicenseData::operator<<(OutputBuffer &outputBuffer)
{
	cJSON *root = cJSON_CreateObject();
	cJSON *license = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "license", license);

	cJSON_AddStringToObject(license, "machine_unique_id", m_machineUniqueId.c_str());
	cJSON_AddStringToObject(license, "version", m_licenseVersion.c_str());

	cJSON_AddStringToObject(license, "expires", m_expirationDateString.c_str());
	cJSON_AddNumberToObject(license, "license_type", (int)m_licenseType);

	cJSON_AddStringToObject(root, "signature", m_signature.c_str());
	
	cJSON_AddStringToObject(root, "cert_encoded", m_certEncoded.c_str());

	if (m_licenseType != LicenseData::TEMPORARY)
	{
		cJSON_AddStringToObject(license, "license_number", m_licenseNumber.c_str());
		cJSON_AddStringToObject(license, "device_id", m_deviceId.c_str());

		cJSON_AddStringToObject(license, "owner", m_ownerName.c_str());
		cJSON_AddStringToObject(license, "phone_number", m_email.c_str());
	}

	char *ptr = cJSON_PrintUnformatted(root);
	outputBuffer.putBytes((uint8_t *)ptr, strlen(ptr));
	cJSON_Delete(root);
	
	delete ptr;

	return *this;
}

int LicenseData::checkExpirationPeriodDays() const
{
	//get expiration time from json, and convert it to std::tm
	std::tm expirationTime_tm = { 0 };
	int dateAsString = atoi(m_expirationDateString.c_str());

	expirationTime_tm.tm_year = dateAsString / 10000 - 1900;
	expirationTime_tm.tm_mon = (dateAsString % 10000) / 100 - 1;
	expirationTime_tm.tm_mday = dateAsString % 100;
#ifdef __ANDROID__
//on android use 64 bit version, since 32 bit does not support dates beyond 2038
	time64_t expirationTime_time_t = mktime64(&expirationTime_tm);
#else
    time_t expirationTime_time_t = mktime(&expirationTime_tm);
#endif
    if (expirationTime_time_t < 0)
	{
        m_lastError = JsonParsingError;
		log(Error, "Failed to convert the expire tag to time_t");
		return -1;
	}

	auto now = std::chrono::system_clock::now();
#ifdef __ANDROID__
	time64_t now_time_t = std::chrono::system_clock::to_time_t(now);
#else
    time_t now_time_t = std::chrono::system_clock::to_time_t(now);
#endif
	//calculate the difference between the current time and the expiration time. 
	//if it is positive, the license is still valid. 
	//Otherwise(zero or negative) - the license has expired

#ifdef __ANDROID__
    time64_t diff = expirationTime_time_t - now_time_t;
#else
    time_t diff = difftime(expirationTime_time_t, now_time_t);
#endif
	if (diff < 0)
	{
        m_lastError = Expired;
	    log(Debug, "license is expired");
		return Expired;
	}

	return diff / 86400;//divide by the number of seconds in a day
}

LicenseData & operator >> (InputBuffer &inputBuffer, LicenseData &data)
{
	uint8_t *bytes;
	size_t readBytes;
	if (inputBuffer.getBytes(&bytes, readBytes))
	{
		cJSON *parsed = cJSON_Parse((char *)bytes);
		delete[] bytes;

		if (parsed != NULL)
		{
			cJSON *jsonSignatureNode = cJSON_GetObjectItem(parsed, "signature");
			cJSON *jsonCertEncodedNode = cJSON_GetObjectItem(parsed, "cert_encoded");

			cJSON *licenseNode = cJSON_GetObjectItem(parsed, "license");
			cJSON *jsonLicenseType = cJSON_GetObjectItem(licenseNode, "license_type");
			cJSON *jsonMachineId = cJSON_GetObjectItem(licenseNode, "machine_unique_id");
			cJSON *jsonVersion = cJSON_GetObjectItem(licenseNode, "version");
			cJSON *jsonExpiry = cJSON_GetObjectItem(licenseNode, "expires");

			int type = jsonLicenseType->valueint;
			//the temp license has lesser set of tags
			
			if (jsonSignatureNode && jsonCertEncodedNode
				&& jsonMachineId &&  jsonExpiry && jsonExpiry && jsonVersion)
			{
				data.m_signature = jsonSignatureNode->valuestring;
				data.m_machineUniqueId = jsonMachineId->valuestring;
				data.m_expirationDateString = jsonExpiry->valuestring;
				data.m_certEncoded = jsonCertEncodedNode->valuestring;
				data.m_licenseVersion = jsonVersion->valuestring;
			}
			else
			{
				data.m_valid = false;
                data.m_lastError = JsonParsingError;
				log(Error, "failed to find json tags");
				return data;
			}
			if (type != LicenseData::TEMPORARY)
			{
				cJSON *jsonDeviceId = cJSON_GetObjectItem(licenseNode, "device_id");
				cJSON *jsonLicenseNumber = cJSON_GetObjectItem(licenseNode, "license_number");
				cJSON *jsonOwner = cJSON_GetObjectItem(licenseNode, "owner");
				cJSON *jsonPhoneNumber = cJSON_GetObjectItem(licenseNode, "phone_number");
				cJSON *jsonEmail = cJSON_GetObjectItem(licenseNode, "email");

				if (jsonDeviceId && jsonLicenseNumber && jsonOwner
					&& jsonPhoneNumber && jsonEmail && jsonExpiry)
				{
					data.m_deviceId = jsonDeviceId->valuestring;
					data.m_licenseNumber = jsonLicenseNumber->valuestring;
					data.m_ownerName = jsonOwner->valuestring;
					data.m_phoneNumber = jsonPhoneNumber->valuestring;
					data.m_email = jsonEmail->valuestring;
				}
				else
				{
					data.m_valid = false;
                    data.m_lastError = JsonParsingError;
					log(Error, "failed to find json tags");
					return data;
				}
			}

			std::unique_ptr<char> pJsonLicenseString(cJSON_PrintUnformatted(licenseNode));

			int rv = checkSignature(data.m_signature, pJsonLicenseString.get(), (long)strlen(pJsonLicenseString.get()), data.m_certEncoded);
			if (rv != Success)
			{
				log(Error, "signature mismatch");
				data.m_valid = false;
				data.m_signatureValid = false;
                data.m_lastError = rv;
			}
			cJSON_Delete(parsed);
		}
		else
		{
			log(Error, "failed to parse license.json");
			data.m_valid = false;
            data.m_lastError = JsonParsingError;
		}
	}
	else
	{
		log(Debug, "license missing");
		data.m_valid = false;
        data.m_lastError = MissingLicense;
	}

	return data;
}

ActivateLicenseData::ActivateLicenseData(LicenseData::LicenseType licenseType, const std::string &deviceId, const std::string &licenseJsonFile
	, const std::string &productId, const std::string &swVersion)
	: LicenseData(licenseType, deviceId, licenseJsonFile)
	, m_productId(productId)
	, m_swVersion(swVersion)
{

}

