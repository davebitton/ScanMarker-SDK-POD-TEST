#ifndef __LICENSING_H__
#define __LICENSING_H__

#include "LicenseSettings.h"

//return values

enum LicenseStatus
{
	LicenseValid = 0,
	GeneralEror = -101, 
	MissingLicense = -102,
	SignatureMismatch =-103,
	WrongDevice =-104,
	Expired = -105,
	DeviceMismatch = -106,
	MachineIdMismatch = -107,
    BadCertificate = -108
};


//return values

enum ServerResult
{
	Success = 0,
	DeviceNotSupported = -1,
	TooManyLicenses = -2,
	InvalidGeo = -3,
	GeoNotSupported = -4,
	ProductNotSupported =-5,
	VersionNotSupported =-6, 
	PlatformNotSupported =-7,
	DeviceNotFound = -8,
	LicenseNotFound = -9,
	NetworkError = -10,
	GeneralError = -11,
	InvalidInput = -12,
	JsonParsingError = -13,
	CertificateMissingOnServer = -14,
	CertificateMissing = -15,
	PublicKeyMissing = -16
};


class OutputBuffer;

int acquire_license(ActivateLicenseData &acquireLicenseData, OutputBuffer &buffer);

class InputBuffer;

int check_license(const LicenseData *pLicenseData, InputBuffer &buffer);

int deactivate_license(const LicenseData &deactivateData);

int check_expiration_period_days(InputBuffer &buffer);

//new API for the Multithread application

bool check_license(const char *path2FileOrFolder);

extern "C"
{
bool check_license(const char *path2FileOrFolder, const char *scannerSerial);
}
int getRemainingDays(const char *path2FileOrFolder);

int getRemainingDays(const char *path2FileOrFolder, const char *scannerSerial);

#ifdef __ANDROID__
extern "C"
{
    int check_license_android();
}
#endif//__ANDROID__

#endif // #ifndef __LICENSING_H__
