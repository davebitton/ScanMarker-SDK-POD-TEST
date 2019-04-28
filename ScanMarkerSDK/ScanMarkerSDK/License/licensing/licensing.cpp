// test2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>

#include "licensing.h"
#include "LicenseChecker.h"

#include "PlatformDefine.h"

#include <regex>
#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <stdio.h>

#include "LicenseSettings.h"

#include "LicensingLog.h"
#include "LicensingIO.h"

#include "LicensingCrypto.h"
#include "LicensingMemory.h"

#ifdef __ANDROID__
#include <time64.h>
#endif
void destroyJsonRoot(cJSON *root)
{
	//DEBUG ON ANDROID
	//return;
#ifndef __ANDROID__
    log(Warn, "destroy start");
    if(root != 0)
	    cJSON_Delete(root);
    log(Warn, "destroy end");
#endif
}

#ifndef __ANDROID__
#include <curl/curl.h>
typedef std::unique_ptr<CURL, void(*)(CURL *)> CurlSmartPtr;
#endif


#if (USE_MACHINE_ID_CODE==1)

bool validateFingerprint(std::string &testString)
{
	// unpack the given string. parse failures return false.
	//KxCbuf testString = testIdString;
	char *pBuffer = const_cast<char *>(testString.c_str());
    
    //we must take only 5 last tokens. strtok will bring wrong values in case the computer name has dashes, for example, Yurys-MBP
    std::vector<std::string> strings;
    std::istringstream f(testString);
    std::string s;
    while (getline(f, s, '-')) {
        strings.push_back(s);
    }
    if (strings.size() < 6)
    {
        log(Error, "validateFingerprint failed, the machine id is too short");
        if (testString.size() > 0)
            log(Error, testString.c_str());
        
        return false;
    }
	char* testName = strtok(pBuffer, "-");
	if (!testName) 
		return false;

	u16 testId[5];
    
	for (u32 i = 0; i < 5; i++)
	{
		char* testNum = strtok(NULL, "-");
		if (!testNum) 
			return false;

		testId[i] = (u16)(strtol(testNum, NULL, 16));
	}
    int i = 4;
    for (std::vector<std::string>::reverse_iterator rit = strings.rbegin(); i >= 0; rit++)
    {
        testId[i--] = (u16)(strtol(rit->c_str(), NULL, 16));
    }
	unsmear(testId);

	// make sure this id is valid - by looking at the checkdigits
	u16 check = 0;
	for (u32 i = 0; i < 4; i++)
		check += testId[i];
	if (check != testId[4]) return false;

	// get the current system information
	u16 systemId[5];
	memcpy(systemId, computeSystemUniqueId(), sizeof(systemId));
	unsmear(systemId);

	// now start scoring the match
	u32 score = 0;

	for (u32 i = 0; i < 4; i++)
		if (testId[i] == systemId[i])
			score++;
	
	std::string machineNameStr = getMachineName();
	if (machineNameStr == testName)
		score++;

	// if we score 3 points or more then the id matches.
	return (score >= 3) ? true : false;
}
#endif//USE_MACHINE_ID_CODE

int check_license(const LicenseData *pLicenseData, InputBuffer &buffer)
{
	LicenseChecker checker;
	return checker.checkLicense(buffer) ? Success : checker.getLastError();
}

static size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *data) 
{
	std::string *pString = reinterpret_cast<std::string *>(data);
	size_t realsize = size * nmemb;
	pString->append((const char *)ptr);
	return realsize;
}


int acquire_license(ActivateLicenseData &activateLicenseData, OutputBuffer &buffer)
{
	log(Warn, "acquire_license start");

	if (activateLicenseData.getDeviceId().empty() 
		|| activateLicenseData.getProductId().empty() || activateLicenseData.getSwVersion().empty())
	{
		return InvalidInput;
	}

	JsonSmartPtr root(cJSON_CreateObject(),destroyJsonRoot);
	
	cJSON_AddStringToObject(root.get(), "scanner_serial", activateLicenseData.getDeviceId().c_str());
	cJSON_AddStringToObject(root.get(), "machine_unique_id", activateLicenseData.getMachineUniquieId().c_str());
	cJSON_AddStringToObject(root.get(), "version", activateLicenseData.getLicenseVersion().c_str());
	cJSON_AddNumberToObject(root.get(), "license_type", (int)activateLicenseData.getLicenseType());
	cJSON_AddStringToObject(root.get(), "instance_id", "future");
	cJSON_AddStringToObject(root.get(), "product_id", activateLicenseData.getProductId().c_str());
	cJSON_AddStringToObject(root.get(), "sw_version", activateLicenseData.getSwVersion().c_str());
	cJSON_AddStringToObject(root.get(), "email", activateLicenseData.getEmail().c_str());
	cJSON_AddStringToObject(root.get(), "owner", activateLicenseData.getOwnerName().c_str());
	cJSON_AddStringToObject(root.get(), "phone_number", activateLicenseData.getPhoneNumber().c_str());
#ifdef _WIN32
	cJSON_AddStringToObject(root.get(), "platform", "windows");
#elif defined __APPLE__
#ifdef LICENSE_IOS
	cJSON_AddStringToObject(root.get(), "platform", "ios");
#else
    cJSON_AddStringToObject(root.get(), "platform", "macosx");
#endif
#elif defined __ANDROID__
	cJSON_AddStringToObject(root.get(), "platform", "android");
#endif
	typedef std::map<std::string, std::string> ExtraParamsMap;
	const ExtraParamsMap &mapRef = activateLicenseData.getExtraPropertiesConst();
	if (!mapRef.empty()) 
	{
		cJSON *extra =	cJSON_AddObjectToObject(root.get(), "extras");
		for (ExtraParamsMap::const_iterator it = mapRef.begin(); it != mapRef.end(); it++) 
		{
			cJSON_AddStringToObject(extra, it->first.c_str(), it->second.c_str());
		}
	}

	//cJSON *geo = cJSON_CreateObject();

	//cJSON_AddNumberToObject(geo, "latitude", 32.000);
	//cJSON_AddNumberToObject(geo, "longitude", 34.000);
	
	//cJSON_AddItemToObject(root.get(), "geo", geo);
	
	std::unique_ptr<char> pJsonString (cJSON_Print(root.get()));
#ifdef __ANDROID__
	root.reset(0);
#endif
	int returnValue = Success;
	std::string outputBuffer;
	
	std::string url(LicenseSettings::instance()->getActivationUrl());
	std::string logMessage("acquire_license(): activation url is ");
	logMessage.append(url);
	
	log(Debug, logMessage.c_str());
#ifdef __ANDROID__
    returnValue = LicenseSettings::instance()->sendRequest(LicenseSettings::instance()->getActivationUrl(), pJsonString.get(), outputBuffer);
	/* Check for errors */
	if (returnValue != Success)
	{
		char errorBuf[256] = {0};
		snprintf(errorBuf, 255, "acquire_license: android web request failed: %d", returnValue);
		log(Error, errorBuf);
		returnValue = NetworkError;
	}
#else//not __ANDROID__
	CurlSmartPtr curl(curl_easy_init(), curl_easy_cleanup);

	if (curl) {
		curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
		
		curl_easy_setopt(curl.get(), CURLOPT_SSL_OPTIONS, CURLSSLOPT_ALLOW_BEAST |
			CURLSSLOPT_NO_REVOKE);

		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, (long)strlen(pJsonString.get()));
		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, pJsonString.get());
		struct curl_slist *list = NULL;
		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, list);
#ifdef _WIN32
		curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-windows-32bit");
#elif defined __APPLE__
        curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-macosx");
//#elif defined __ANDROID__
//        curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-android");
        //On Android, we need the root cert in order to get it working, see the ref:
        //https://github.com/gcesarmza/curl-android-ios/issues/5
//        curl_easy_setopt(curl.get(),CURLOPT_CAINFO, LicenseSettings::instance()->getRootCertPath().c_str());
#endif
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, (void *)&outputBuffer);
		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, (void *)0);

        CURLcode res = CURLE_OK;

		/* Perform the request, res will get the return code */
		try
		{
			res = curl_easy_perform(curl.get());
		}
		catch (...) 
		{
			//prevent memory leaks caused by exceptions
		}
		curl_slist_free_all(list);

		/* Check for errors */
		if (res != CURLE_OK)
		{
			char errorBuf[256] = {0};
			snprintf(errorBuf, 255, "curl_easy_perform() failed: %s",
					 curl_easy_strerror(res));
			log(Error, errorBuf);
			returnValue = NetworkError;
		}
#endif//__ANDROID_
		else //check the signature and store the license on the disk
		{
			JsonSmartPtr parsed(cJSON_Parse(outputBuffer.c_str()), destroyJsonRoot);

			if (parsed.get() != NULL)
			{
				cJSON * result = cJSON_GetObjectItem(parsed.get(), "result");
				if (result)
				{
					int resVal = result->valueint;
					if (resVal == 1)
					{
						cJSON *license = cJSON_GetObjectItem(parsed.get(), "license");
						if (license) 
						{
							cJSON *signature = cJSON_GetObjectItem(parsed.get(), "signature");
							cJSON *pCertEncoded = cJSON_GetObjectItem(parsed.get(), "cert_encoded");

							if (signature && signature->valuestring && pCertEncoded && pCertEncoded->valuestring)
							{
								//base 64 decode the value
								std::string encoded(signature->valuestring);

								ByteSmartPtr ptr(cJSON_PrintUnformatted(license), free);
								std::string licenseStr(ptr.get());

								int signatureMatches = checkSignature(encoded, licenseStr.c_str(), licenseStr.size(), pCertEncoded->valuestring);
								
								if (signatureMatches == Success)
								{
									//get rid of the result tag in the serialized JSON, keep only license & signature tags

									cJSON_DetachItemFromObject(parsed.get(), "result");
									cJSON_DetachItemFromObject(parsed.get(), "description");

									ByteSmartPtr pJsonString2(cJSON_PrintUnformatted(parsed.get()),free);
									int bufferLen = strlen(pJsonString2.get());
									if (!buffer.putBytes((uint8_t *)pJsonString2.get(), bufferLen))
									{
										log(Error, "acquire_license(): failed to write the license data to the output stream");
									}

									log(Debug, "License acquisition succeeded");
                                    //add license number to the LicenseData, testing on Android
                                    cJSON *license_number = cJSON_GetObjectItem(license, "license_number");
                                    if(license_number)
                                    {
                                        activateLicenseData.setLicenseNumber(license_number->valuestring);
                                    }
#ifdef __ANDROID__
                                    parsed.reset(0);
#endif
								}
								else 
								{
									log(Error, "License acquisition failed - signature mismatch");
									returnValue = SignatureMismatch;
								}
							}
							else 
							{
								log(Error, "signature not found in server reply");
								returnValue = JsonParsingError;
							}
						}
						else 
						{
							log(Error, "license not found in server reply");
							returnValue = JsonParsingError;
						}
					}
					else//error, get the description and exit 
					{
						cJSON *jsonErrorDescription = cJSON_GetObjectItem(parsed.get(), "description");
						char buf[256] = { 0 };
						if (jsonErrorDescription) 
						{
							snprintf(buf, 255, "License acquisition failed. Server returned error: %s", jsonErrorDescription->valuestring);
							log(Error, buf);
						}
						else
						{
							log(Error, "License acquisition failed. Server returned error: null");
						}			
						
						returnValue = resVal;
					}
				}
				else 
				{
					log(Error, "Failed to find result tag in the response");
					returnValue = JsonParsingError;
				}
			}
			else 
			{
				log(Error, "Failed to parse json response from the server");
				returnValue = JsonParsingError;
			}
		}
#ifndef __ANDROID__
	}
#endif//__ANDROID__
    log(Warn, "acquire_license end");
	return returnValue;
}

int deactivate_license(const LicenseData &deactvateLicenseData)
{
	JsonSmartPtr root(cJSON_CreateObject(),destroyJsonRoot);
	cJSON_AddStringToObject(root.get(), "device_id", deactvateLicenseData.getDeviceId().c_str());
	cJSON_AddStringToObject(root.get(), "machine_unique_id", deactvateLicenseData.getMachineUniquieId().c_str());
	cJSON_AddStringToObject(root.get(), "version", deactvateLicenseData.getLicenseVersion().c_str());
	cJSON_AddStringToObject(root.get(), "license_number", deactvateLicenseData.getLicenseNumber().c_str());
	std::unique_ptr<char>pJsonString(cJSON_Print(root.get()));

	int returnValue = Success;
	std::string outputBuffer;
#ifdef __ANDROID__
    returnValue = LicenseSettings::instance()->sendRequest(LicenseSettings::instance()->getDeactivationUrl(), pJsonString.get(), outputBuffer);
    /* Check for errors */
    if (returnValue != Success)
    {
        char errorBuf[256] = {0};
        snprintf(errorBuf, 255, "acquire_license: android web request failed: %d", returnValue);
        log(Error, errorBuf);
        returnValue = NetworkError;
    }
#else
	CurlSmartPtr curl(curl_easy_init(), curl_easy_cleanup);
	if (curl) {
		std::string url(LicenseSettings::instance()->getDeactivationUrl());
		std::string logMessage("deactivate_license(): activation url is ");
		logMessage.append(url);

		curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, (long)strlen(pJsonString.get()));
		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, pJsonString.get());
		struct curl_slist *list = NULL;
		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, list);
#ifdef _WIN32
		curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-windows-32bit");
#elif defined __APPLE__
    #if TARGET_OS_MAC
        curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-macosx");
    #else
        curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-ios");
    #endif
#elif defined __ANDROID__
        curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "libcurl-android");
        //On Android, we need the root cert in order to get it working, see the ref:
        //https://github.com/gcesarmza/curl-android-ios/issues/5
        curl_easy_setopt(curl.get(),CURLOPT_CAINFO, LicenseSettings::instance()->getRootCertPath().c_str());
#endif
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, (void *)&outputBuffer);
		curl_easy_setopt(curl.get(), CURLOPT_SSL_OPTIONS, CURLSSLOPT_ALLOW_BEAST |
			CURLSSLOPT_NO_REVOKE);

		/* Perform the request, res will get the return code */
		CURLcode res;
		try
		{
			res = curl_easy_perform(curl.get());
		}
		catch (...) 
		{
		
		}
		curl_slist_free_all(list);

		
		/* Check for errors */
          		if (res != CURLE_OK)
		{
			char errorBuf[256] = { 0 };
			snprintf(errorBuf, 255, "curl_easy_perform() failed: %s",
				curl_easy_strerror(res));
			log(Error, errorBuf);
			returnValue = NetworkError;
		}
#endif//__ANDROID__
		else
		{
			JsonSmartPtr parsed(cJSON_Parse(outputBuffer.c_str()),destroyJsonRoot);
			if (parsed != NULL)
			{
				cJSON * result = cJSON_GetObjectItem(parsed.get(), "result");
				if (result)
				{
					int resVal = result->valueint;
					if (resVal == 1)
					{
						log(Debug, "license deactivation succeeded");
					}
					else//error, get the description and exit 
					{
						char *description = 0;
						cJSON *jsonErrorDescription = cJSON_GetObjectItem(parsed.get(), "description");
						char buf[256] = { 0 };
						if (jsonErrorDescription)
						{
							snprintf(buf, 255, "License deactivation failed. Server returned error: %s", jsonErrorDescription->valuestring);
							log(Error, buf);
						}
						else
						{
							log(Error, "License deactivation failed. Server returned error: null");
						}

						returnValue = resVal;
					}
				}
				else
				{
					log(Error, "Failed to find result tag in the response");
					returnValue = JsonParsingError;
				}
				
			}
			else
			{
				std::string errorMessage("Failed to parse json response from the server: ");
				errorMessage.append(outputBuffer.c_str());

				log(Error, errorMessage.c_str());
				returnValue = JsonParsingError;
			}
#ifndef __ANDROID__
		}
#endif//__ANDROID__
	}
	return returnValue;
}

int check_expiration_period_days(InputBuffer &buffer) 
{
	uint8_t *bytes = 0;
	size_t bytesRead = 0;
	
	if (buffer.getBytes(&bytes, bytesRead))
	{
		JsonSmartPtr parsed(cJSON_Parse((char *)bytes), destroyJsonRoot);
		delete[] bytes;

		if (parsed != NULL)
		{
			cJSON * licenseTag = cJSON_GetObjectItem(parsed.get(), "license");
			if (licenseTag == NULL)
			{
				log(Error, "failed to find license tag");
				return InvalidInput;
			}
			int rv = LicenseValid;
			
			cJSON *jsonExpiration = cJSON_GetObjectItem(licenseTag, "expires");
			if (!jsonExpiration) 
			{
				log(Error, "Failed to extract expires tag from json");
				return InvalidInput;
			}
			char *expirationStr = jsonExpiration->valuestring;
			if (!expirationStr || strlen(expirationStr) != 8 ) 
			{
				log(Error, "Failed to extract expires tag value from json");
				return InvalidInput;
			}
			//get expiration time from json, and convert it to std::tm
			std::tm expirationTime_tm = { 0 };
			int dateAsString = atoi(expirationStr);
			
			expirationTime_tm.tm_year = dateAsString / 10000 - 1900;
			expirationTime_tm.tm_mon = (dateAsString %10000) / 100 - 1;
			expirationTime_tm.tm_mday = dateAsString % 100;
            //on android use 64 bit version, since 32 bit does not support dates beyond 2038
#ifdef __ANDROID__
            time64_t expirationTime_time_t = mktime64(&expirationTime_tm);
#else
            time_t expirationTime_time_t = mktime(&expirationTime_tm);
#endif
			if (expirationTime_time_t < 0) 
			{
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
			double diff = expirationTime_time_t - now_time_t;
#else
            double diff = difftime(expirationTime_time_t, now_time_t);
#endif
			if (diff < 0) 
			{
				return Expired;
			}
			
			return diff / 86400;//divide by the number of seconds in a day
		}
		else
		{
			log(Error, "failed to parse license.json");
			return JsonParsingError;
		}
	}
	else
	{
		log(Error, "license missing, or file is corrupt");
		return MissingLicense;
	}
}

bool check_license(const char *path_to_file_or_folder)
{
	LicenseChecker checker;
	return checker.checkLicense(path_to_file_or_folder);
}

bool check_license(const char *path_to_file_or_folder, const char *scanner_serial)
{
	LicenseWithDeviceIdChecker checker(scanner_serial);
	return checker.checkLicense(path_to_file_or_folder);
}

int getRemainingDays(const char *path_to_file_or_folder) 
{
	LicenseChecker checker;
	return checker.getRemainingDays(path_to_file_or_folder);
}

int getRemainingDays(const char *path_to_file_or_folder, const char *activation_code_or_scanner_serial)
{
	LicenseWithDeviceIdChecker checker(activation_code_or_scanner_serial);
	return checker.getRemainingDays(path_to_file_or_folder);
}

#ifdef __ANDROID__
int check_license_android()
{
    if(!LicenseSettings::instance()->isInitialized())
    {
        log(Error, "check_license_android: LicenseSettigns is not initialized, exiting");
        return InvalidInput;
    }
    //first, check whether the license file exists
    std::string scannerId = LicenseSettings::instance()->getScannerId();
    std::string licenseFileName = LicenseSettings::instance()->getLicenseFileName();
    LicenseWithDeviceIdChecker checker(scannerId.c_str());
    
    if(checker.checkLicense(licenseFileName.c_str()))
    {
        log(Debug, "check_license_android: checkLicense succeeded");
        return 0;
    }
    else //try to acquire a license
    {
        std::ostringstream logMsgStream;
        logMsgStream << "check_license_android: license check failed. Last error: ";
        logMsgStream << checker.getLastError() << "Try to acquire the license";
        
        log(Warn, logMsgStream.str().c_str());
                
        ActivateLicenseData activateData;
        activateData.setDeviceId(scannerId.c_str());
        activateData.setLicenceType(LicenseData::DEVICE_SERIAL_NUM);
        activateData.setProductId("Scanmarker");
        activateData.setSwVersion("4.2");
        std::string machineUniqueId = LicenseSettings::instance()->getMachineUniqueId();
        activateData.setMachineUniqueId(machineUniqueId);
        activateData.setOwnerName("android");
        activateData.setEmail("android@topscan.com");
        activateData.setPhoneNumber("none");
        
        FileOutputBuffer outBuffer(licenseFileName.c_str());
        
        return acquire_license(activateData, outBuffer);
    }
}
#endif//__ANDROID__

