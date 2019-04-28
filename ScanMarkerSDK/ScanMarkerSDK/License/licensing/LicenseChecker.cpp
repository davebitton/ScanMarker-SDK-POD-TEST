
#include "LicenseChecker.h"
#include <cstring>
#include "LicensingLog.h"
#include "licensing.h"
#include "LicensingIO.h"

#include <errno.h>

#include "PlatformDefine.h"

#include <sstream> // stringstream

LicenseChecker::LicenseChecker()
: m_lastError(Success)
{
}


LicenseChecker::~LicenseChecker()
{
}

bool LicenseChecker::checkLicense(const char *path_to_file_or_folder)
{
    std::string pathStr(path_to_file_or_folder);
     
	struct stat statStr;
	if (0 != stat(pathStr.c_str(), &statStr))
	{
        m_lastError = MissingLicense;
		log(Warn, "check_license: failed to find file");
        log (Error, strerror(errno));
		return false;
	}
	if (S_ISREG(statStr.st_mode)) //regular file
	{
		return checkLicenseForFile(path_to_file_or_folder);
	}
#ifdef __ANDROID__//readdir crashes on Android
    else
    {
        return MissingLicense;
    }
#else
	else if (S_ISDIR(statStr.st_mode))
	{
		DIR *dfd = 0;
		if ((dfd = opendir(path_to_file_or_folder)) == NULL)
		{
			std::string errorMsg("Can't open ");
			errorMsg.append(path_to_file_or_folder);
			log(Error, errorMsg.c_str());
			return false;
		}
		struct dirent *dp;

		while (true)
		{
            dp = readdir(dfd);
            
            if (dp == NULL)
            {
                break;
            }
			struct stat stbuf;
            std::string fullPathStr (pathStr);
            fullPathStr.append(SLASH);
            fullPathStr.append(dp->d_name);
			if (stat(fullPathStr.c_str(), &stbuf) == -1)
			{
				log(Warn, "Unable to stat file:");
				log(Warn, dp->d_name);
				continue;
			}
			if (S_ISREG(stbuf.st_mode))
			{
				//check extension, must be .json
				int len = strlen(dp->d_name);
				if (len > 5 && !strncmp(dp->d_name + len - 5, ".json", 5))
				{
					std::string fullFileName(path_to_file_or_folder);
					fullFileName.append(SLASH);
					fullFileName.append(dp->d_name);
					if (checkLicenseForFile(fullFileName.c_str()))
					{
						log(Debug, "found valid license, file ");
						log(Debug, dp->d_name);
						closedir(dfd);
						return true;
					}
				}
			}
		}
		closedir(dfd);
	}
	return false;
#endif//__ANDROID__
}

bool LicenseChecker::checkLicense(InputBuffer &buffer)
{
	LicenseData licenseData;
	buffer >> licenseData;

	if (!licenseData.isValid())//this checks the license signature, too
	{
        m_lastError = licenseData.getLastError();
		return false;
	}

	//DEVICE_SERIAL_NUM requires serial num input
	if (licenseData.getLicenseType() == LicenseData::DEVICE_SERIAL_NUM)
	{
		log(Error, "Device serial number license requires serial number input. Use LicenseWithDeviceIdChecker instead");
        m_lastError = WrongDevice;
		return false;
	}

#if (USE_MACHINE_ID_CODE==1)//win, mac
	std::string uniqueId = getSystemUniqueId();
	if (!validateFingerprint(uniqueId))
#else//iOS, Android
	if (LicenseSettings::instance()->getMachineUniqueId() != licenseData.getMachineUniquieId())
#endif //USE_MACHINE_ID_CODE==1
	{
        m_lastError = MachineIdMismatch;
#ifdef __ANDROID__
        std::ostringstream logMsgStream;
        logMsgStream << "machine id mismatch - acquire a new license for this machine. expected machine id: ";
        logMsgStream << LicenseSettings::instance()->getMachineUniqueId();
        logMsgStream << "actual machine id: " << licenseData.getMachineUniquieId() << ".Try to acquire the license";

		log(Error, logMsgStream.str().c_str());
#else
        log(Error, "machine id mismatch - acquire a new license for this machine");
#endif//__ANDROID__
		return false;
	}
	if (licenseData.isExpired())
	{
        m_lastError = Expired;
		log(Error, "license is expired");
		return false;
	}

	return true;
}

//here we assume that the file exists
bool LicenseChecker::checkLicenseForFile(const char *path2File)
{
	FileInputBuffer fileInput(path2File);
	return checkLicense(fileInput);
}

int LicenseChecker::getRemainingDaysForFile(const char *path2File)
{
	FileInputBuffer fileInput(path2File);
	LicenseData licenseData;
	fileInput >> licenseData;
	return licenseData.checkExpirationPeriodDays();
}

int LicenseChecker::getRemainingDays(const char *path_to_file_or_folder)
{
    log(Debug, "LicenseChecker::getRemainingDays start");
    log(Debug, "path to file or folder is");
    log(Debug, path_to_file_or_folder);

    std::string pathStr(path_to_file_or_folder);
    //pathStr = std::regex_replace(pathStr, std::regex(" "), "\\ ");

    log(Debug, "LicenseChecker::getRemainingDays after regex_replace");
    log(Debug, "LicenseChecker::getRemainingDays pathStr");
    log(Debug, pathStr.c_str());

	struct stat statStr;
	if (0 != stat(pathStr.c_str(), &statStr))
	{
        m_lastError = MissingLicense;
		log(Warn, "check_license: failed to find file");
		return false;
	}
    log(Debug, "LicenseChecker::getRemainingDays after stat 1");
	if (S_ISREG(statStr.st_mode)) //regular file
	{
		return getRemainingDaysForFile(pathStr.c_str());
	}
#ifdef __ANDROID__//readdir crashes on Android
	else
	{
		return MissingLicense;
	}
#else
	else if (S_ISDIR(statStr.st_mode))
	{
		int maxRemainingDays = 0;

		DIR *dfd = 0;
		if ((dfd = opendir(pathStr.c_str())) == NULL)
		{
            m_lastError = GeneralEror;
			std::string errorMsg("Can't open ");
			errorMsg.append(pathStr);
			log(Error, errorMsg.c_str());
			return false;
		}

        log(Debug, "LicenseChecker::getRemainingDays after opendir");

		struct dirent *dp;

		while ((dp = readdir(dfd)) != NULL)
		{
            log(Debug, "LicenseChecker::getRemainingDays after readdir");
            if(dp->d_name == 0)
            {
                log(Debug, "LicenseChecker::getRemainingDays got dir entry name = null");
                continue;
            }

            if(0 == strlen (dp->d_name))
            {
                log(Debug, "LicenseChecker::getRemainingDays got dir entry name is empty");
                continue;
            }

            if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
            {
                log(Debug, "LicenseChecker::getRemainingDays skip . and .. dir");
                continue;
            }

            log(Debug, "LicenseChecker::getRemainingDays dp->d_name");
            log(Debug, dp->d_name);

            struct stat stbuf;
            log(Debug, "LicenseChecker::getRemainingDays before fullPathStr copy ctr ");
            std::string fullPathStr (pathStr);
            log(Debug, "LicenseChecker::getRemainingDays before fullPathStr append ");
            fullPathStr.append(SLASH);
            log(Debug, "LicenseChecker::getRemainingDays before fullPathStr append dp->d_name");
            fullPathStr.append(dp->d_name);

            log(Debug, "LicenseChecker::getRemainingDays before stat, path is ");
            log(Debug, fullPathStr.c_str());

			if (stat(fullPathStr.c_str(), &stbuf) != 0)
			{
				log(Warn, "Unable to stat file:");
				log(Warn, dp->d_name);
				continue;
			}
			if (S_ISREG(stbuf.st_mode))
			{
				//check extension, must be .json
				int len = strlen(dp->d_name);
				if (len > 5 && !strncmp(dp->d_name + len - 5, ".json", 5))
				{
					std::string fullFileName(path_to_file_or_folder);
					fullFileName.append(SLASH);
					fullFileName.append(dp->d_name);
					int remainingDaysForFile = getRemainingDaysForFile(fullFileName.c_str());
					maxRemainingDays = (remainingDaysForFile > maxRemainingDays) ? remainingDaysForFile : maxRemainingDays;
				}
			}
		}
		closedir(dfd);
		return maxRemainingDays;
	}
	else 
	{
		log(Error, "got invalid type of file - not file nor folder");
	}
	return MissingLicense;
#endif//__ANDROID__
}

bool LicenseWithDeviceIdChecker::checkLicenseForFile(const char *path2File)
{
	log(Debug, "LicenseWithDeviceIdChecker::checkLicenseForFile start");
	log(Debug, "input file path");
	log(Debug, path2File);

	FileInputBuffer fileInput(path2File);
	LicenseData licenseData;
	fileInput >> licenseData;

	if (!licenseData.isValid())//this checks the license signature, too
	{
        m_lastError = licenseData.getLastError();
		return false;
	}

	//DEVICE_SERIAL_NUM requires serial num input
	if (licenseData.getLicenseType() == LicenseData::DEVICE_SERIAL_NUM && licenseData.getDeviceId() != m_scannerId)
	{
        m_lastError = DeviceMismatch;
		log(Error, "Device serial number mismatch");
		return false;
	}

#if (USE_MACHINE_ID_CODE==1)//win, mac
    std::string uniqueId = getSystemUniqueId();
	if (!validateFingerprint(uniqueId))
#else//iOS, Android
	//TODO: reimplement the machine id logic for mobile
	if (LicenseSettings::instance()->getMachineUniqueId() != licenseData.getMachineUniquieId())
#endif //USE_MACHINE_ID_CODE==1
	{
        m_lastError = MachineIdMismatch;
#ifdef __ANDROID__
        std::ostringstream logMsgStream;
        logMsgStream << "machine id mismatch - acquire a new license for this machine. expected machine id: ";
        logMsgStream << LicenseSettings::instance()->getMachineUniqueId();
        logMsgStream << "actual machine id: " << licenseData.getMachineUniquieId() << ".Try to acquire the license";
        
        log(Error, logMsgStream.str().c_str());
#else
        log(Error, "machine id mismatch - acquire a new license for this machine");
#endif//__ANDROID__
		return false;
	}
	if (licenseData.isExpired())
	{
        m_lastError = Expired;
		log(Error, "license is exprired");
		return false;
	}

	return true;
}

int LicenseWithDeviceIdChecker::getRemainingDaysForFile(const char *path2File)
{
	FileInputBuffer fileInput(path2File);
	LicenseData licenseData;
	fileInput >> licenseData;
	if (licenseData.getLicenseType() == LicenseData::DEVICE_SERIAL_NUM && licenseData.getDeviceId() != m_scannerId) 
	{
        m_lastError = DeviceMismatch;
		log(Warn, "LicenseWithDeviceIdChecker::getRemainingDaysForFile: scanner id mismatch for the license");
		return DeviceMismatch;
	}
	return LicenseChecker::getRemainingDaysForFile(path2File);
}

