//
//  ScanMarkerLicensing.hpp
//  ScanMarker
//
//  Created by dave bitton on 15/01/2019.
//  Copyright © 2019 sion sasson. All rights reserved.

#ifndef ScanMarkerLicensing_h
#define ScanMarkerLicensing_h

#include <Foundation/Foundation.h>

@interface ScanMarkerLicensing : NSObject
- (bool)checkLicense: (NSString*)serialNumber: (NSString*)deviceToken;
- (int)acquireLicense: (NSString*)serialNumber: (NSString*)deviceToken;
- (NSMutableString*) parseSerialNumber:(NSData*)serialNumberData;
- (const char*)getMachineToken;
@end

#endif
