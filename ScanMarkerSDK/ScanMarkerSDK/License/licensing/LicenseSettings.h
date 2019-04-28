#ifndef __LICENCE_SETTINGS_H__
#define __LICENCE_SETTINGS_H__

#include <string>
#include <map>

#include "PlatformDefine.h"

#ifdef __ANDROID__
#include <jni.h>
#endif//__ANDROID__

class LicenseSettings
{
public:
	static LicenseSettings *instance();
#ifdef __ANDROID__
	static void initSettings(JNIEnv *env, const char *activationUrl
                             , const char *deactivationUrl, const char *machineUniqueId
                             , const char *scannerId, const char *licensePath);
    
	int checkSignature(const std::string &signature, const char *msg, int msgLen, const std::string &encodedCert);
    int sendRequest(const std::string &urlIn, const std::string &jsonIn, std::string &jsonOut);
    std::string getLicenseFileName() const;
    
#elif(defined LICENSE_IOS)
	static void initSettings(const char *activationUrl, const char *deactivationUrl, const char *machineUniqueId , const char *scannerId, const char *licensePath);
#else
    static void initSettings(const char *activationUrl, const char *deactivationUrl);
#endif//__ANDROID__
	~LicenseSettings();
	std::string getActivationUrl() const;
	std::string getDeactivationUrl() const;
    bool isInitialized() const {return m_initialized;}
private:
	static LicenseSettings *m_pInstance;

	LicenseSettings();

	std::string m_activationUrl;
	std::string m_deactivationUrl;
    bool m_initialized;
#ifdef __ANDROID__
    JNIEnv *m_env;
#endif//__ANDROID__
#if(USE_MACHINE_ID_CODE !=1)//mobile
	std::string m_machineUniqueId;
    std::string m_scannerId;
    std::string m_licenseFileName;
public:
	std::string getMachineUniqueId()const {return m_machineUniqueId;}
    std::string getScannerId()const {return m_scannerId;}
#endif
};

class InputBuffer;
class OutputBuffer;


//Base class used for verification
class LicenseData
{
	
public:
	enum LicenseType { TEMPORARY = 0xFF, ACTIVATION_CODE = 0xFE, DEVICE_SERIAL_NUM = 0xFD };

	LicenseData();
	LicenseData(LicenseType licenseType, const std::string &deviceId, const std::string &licenseJsonFile);
	
	LicenseData &operator<<(OutputBuffer &outputBuffer);

	const std::string &getDeviceId() const { return m_deviceId; }
	void setDeviceId(const std::string &deviceId) 
	{ 
		if (!deviceId.empty())
			m_deviceId = deviceId; 
	}
	void setLicenceType(LicenseType type) { m_licenseType = type; }
	LicenseType getLicenseType() const { return m_licenseType; }

	const std::string &getMachineUniquieId() const { return m_machineUniqueId; }
	void setMachineUniqueId(const std::string &machineUniquieId) 
	{ 
		if(!machineUniquieId.empty())
			m_machineUniqueId = machineUniquieId; 
	}

	const std::string &getLicenseJsonFile () const { return m_licenseJsonFile;}
	void setLicenseJsonFile(const std::string &licenseJsonFile) 
	{ 
		if(!licenseJsonFile.empty())
			m_licenseJsonFile = licenseJsonFile; 
	}

	const std::string &getOwnerName() const { return m_ownerName; }
	void setOwnerName(const std::string &ownerName) 
	{ 
		if(!ownerName.empty())
			m_ownerName = ownerName; 
	}
	const std::string &getPhoneNumber() const { return m_phoneNumber; }
	void setPhoneNumber(const std::string &phoneNum) 
	{ 
		if(!phoneNum.empty())
			m_phoneNumber = phoneNum; 
	}

	const std::string &getEmail() const { return m_email; }
	void setEmail(const std::string &email) 
	{ 
		if(!email.empty())
			m_email = email; 
	}

	void setLicenseNumber(const std::string &licenseNumber)
	{ 
		if(!licenseNumber.empty())
			m_licenseNumber = licenseNumber; 
	}
	const std::string &getLicenseNumber() const { return m_licenseNumber; }

	void setSignature(const std::string &signature) 
	{ 
		if(!signature.empty())
			m_signature = signature; 
	}
	const std::string &getSignature() const { return m_signature; }

	void setExpirationString(const std::string &expStr) 
	{ 
		if(!expStr.empty())
			m_expirationDateString = expStr; 
	}
	const std::string &getExpirationString() const { return m_expirationDateString; }

	bool isValid() const { return m_valid; }
	bool isSignatureValid() { return m_signatureValid;}
	int checkExpirationPeriodDays() const;
	bool isExpired() const {return checkExpirationPeriodDays() <= 0;}
    int getLastError() const {return m_lastError;}
    
	void setLicenseVersion(const std::string &version)
	{
		m_licenseVersion = version;
	}
	const std::string &getLicenseVersion() const { return m_licenseVersion; }

protected:
	std::string m_deviceId;//this one maybe either device id or serial number, for backward compatiblity
	LicenseType m_licenseType;
	std::string m_ownerName;
	std::string m_email;
	std::string m_phoneNumber;
	std::string m_licenseJsonFile;
	std::string m_machineUniqueId;
	std::string m_licenseNumber;//this is a license number we get from the server
	std::string m_signature;
	std::string m_expirationDateString;
	std::string m_certEncoded;
	std::string m_licenseVersion;

	bool m_valid;
	bool m_signatureValid;
    mutable int m_lastError;
private:
	friend LicenseData &operator >> (InputBuffer &inputBuffer, LicenseData &data);
};

LicenseData &operator >> (InputBuffer &inputBuffer, LicenseData &data);

class ActivateLicenseData
	: public LicenseData
{
public:
	ActivateLicenseData() {}
	ActivateLicenseData(LicenseData::LicenseType licenseType, const std::string &deviceId, const std::string &licenseJsonFile
		, const std::string &productId, const std::string &swVersion);

	void setProductId(const std::string &deviceId) { m_productId = deviceId; }
	const std::string &getProductId() const { return m_productId; }
	void setSwVersion(const std::string &licenseJsonFile) { m_swVersion = licenseJsonFile; }
	const std::string &getSwVersion() const { return m_swVersion; }
	std::map <std::string, std::string> &getExtraProperties() {return m_extraProperties;}
	const std::map <std::string, std::string> &getExtraPropertiesConst() const{ return m_extraProperties; }

private:
	std::string m_productId;
	std::string m_swVersion;
	std::map <std::string, std::string> m_extraProperties;
};

#endif // __LICENCE_SETTINGS_H__

