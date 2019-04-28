//
//  ScanMarkerLicensing.m
//  ScanMarker
//
//  Created by dave bitton on 15/01/2019.
//  Copyright © 2019 sion sasson. All rights reserved.
//

#include "LicenseSettings.h"
#import "licensing.h"
#include "LicensingIO.h"
#include "ScanMarkerLicensing.hpp"

#define USER_DEFAULTS_MACHINE_TOKEN                         @"USER_DEFAULTS_MACHINE_TOKEN"
#define USER_DEFAULTS_DEVICE_SERIAL_NUMBER                         @"USER_DEFAULTS_DEVICE_SERIAL_NUMBER"

@implementation ScanMarkerLicensing 

//use application support directory, as Documents directory does not exist for the iOS simulator
NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
NSString *applicationSupportDirectory = [paths firstObject];
const char *cpath = [applicationSupportDirectory fileSystemRepresentation];
std::string cpp_applicationSupportDirectoryString(cpath);

- (bool)checkLicense: (NSString*)serialNumber : (NSString*)deviceToken{
    const char* SN = serialNumber.UTF8String;
    if (SN == NULL){
        return false;
    }
    LicenseSettings::initSettings("https://lic.scanmarker.com/LicenseWeb/ActivateLicense"
                                  ,"https://lic.scanmarker.com/LicenseWeb/DeactivateLicense", deviceToken.UTF8String, SN, cpp_applicationSupportDirectoryString.c_str());
    bool rv2 = check_license(cpp_applicationSupportDirectoryString.c_str(), SN);
    return rv2;
}

- (int)acquireLicense: (NSString*)serialNumber : (NSString*)deviceToken{
    std::string licenseFilePath = cpp_applicationSupportDirectoryString;
    licenseFilePath.append("/license.json");
    FileOutputBuffer fileOutput(licenseFilePath.c_str());
    ActivateLicenseData activateData;
    activateData.setDeviceId(serialNumber.UTF8String);//use serial instead of the device id for the old version of the qt
    activateData.setLicenceType(LicenseData::DEVICE_SERIAL_NUM);
    activateData.setProductId("ScanMarker");
    activateData.setSwVersion("4.0.1");
    activateData.setMachineUniqueId(deviceToken.UTF8String);
    activateData.setOwnerName("");
    activateData.setEmail("");
    activateData.setPhoneNumber("");
    int rv = acquire_license(activateData, fileOutput);
    return rv;
}


//
//- (const char*)getDeviceSerialNumber {
//    NSString * deviceSN = [[NSUserDefaults standardUserDefaults] valueForKey: USER_DEFAULTS_DEVICE_SERIAL_NUMBER];
//    const char *c = [deviceSN UTF8String];
//    return c;
//}

- (NSMutableString*)parseSerialNumber:(NSData*)serialNumberData {
    NSData *serial0 =  [serialNumberData subdataWithRange:NSMakeRange(0, 4)];
    NSData *serial1 = [serialNumberData subdataWithRange:NSMakeRange(4, 4)];
    NSData *serial2 = [serialNumberData subdataWithRange:NSMakeRange(8, 4)];
    unsigned int value0 = (*(unsigned int*)([serial0 bytes]));
    unsigned int value1 = (*(unsigned int*)([serial1 bytes]));
    unsigned int value2 = (*(unsigned int*)([serial2 bytes]));
    value0 += value2;
    NSMutableData* result = [NSMutableData new];
    [result appendData:[self intToUnicode:value0 len:8]];
    [result appendData:[self intToUnicode:value1 len:4]];
    NSString* uncompressedSerialNumber = [[NSString alloc] initWithData:result encoding:NSASCIIStringEncoding];
    NSMutableString* realSerialNumber = [[NSMutableString alloc] init];
    for(int i = 0; i < uncompressedSerialNumber.length; i++){
        if ([uncompressedSerialNumber characterAtIndex:i] != 0){
            NSString *ch = [NSString stringWithFormat:@"%c", [uncompressedSerialNumber characterAtIndex:i]];
            [realSerialNumber appendString:ch];
        }
    }
    return realSerialNumber;
}

-(NSData*) intToUnicode: (unsigned int)value  len:(int)len{
    //    NSMutableData* result = [[NSMutableData init] initWithCapacity:2*len
    char result[2*len];
    int idx = 0;
    for (idx = 0; idx < len; idx++){
        if (((value >> 28)) < 0xA){
            result[2*idx] = (char)((value >> 28) + '0');
        }
        else{
            result[2*idx] = (char)((value >> 28) + 'A' - 10);
        }
        value = value << 4;
        result[2*idx + 1] = 0;
    } 
    return [NSData dataWithBytes:(const void *)result length:2*len];
}


@end
