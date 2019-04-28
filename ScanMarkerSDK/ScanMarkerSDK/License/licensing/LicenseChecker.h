#ifndef __LICENSE_CHECKER_H__
#define __LICENSE_CHECKER_H__
#include <string>

#include "LicensingIO.h"

class LicenseChecker
{
public:
	LicenseChecker();
	virtual ~LicenseChecker();
	
	bool checkLicense(InputBuffer &buffer);

	bool checkLicense(const char *path2FileOrFolder);
	int getRemainingDays(const char *path2FileOrFolder);
    int getLastError() const {return m_lastError;}
protected:
	virtual int getRemainingDaysForFile(const char *path2FileOrFolder);
    mutable int m_lastError;
private:
	bool checkLicenseForFile(const char *path2File);
	
};

class LicenseWithDeviceIdChecker
	: public LicenseChecker 
{
public:
	LicenseWithDeviceIdChecker(const char *deviceId) 
		: m_scannerId(deviceId)
	{	
	}
protected:
	virtual int getRemainingDaysForFile(const char *path2FileOrFolder);
private:
	bool checkLicenseForFile(const char *path2File);
	std::string m_scannerId;

};

#endif//__LICENSE_CHECKER_H__

