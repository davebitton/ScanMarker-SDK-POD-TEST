//
//  LicenseLoggingIOS.cpp
//  LicensingIos
//
//  Created by Yury Markman on 16/09/2018.
//  Copyright © 2018 Scanmarker. All rights reserved.
//

#include "LicensingLog.h"
#include <string>
#import <Foundation/Foundation.h>

void log(int level, const char *message)
{
    const static std::string LogLevelStrings[3] = { "DEBUG", "WARN", "ERROR" };
    NSString *logMsg = [NSString stringWithFormat:@"%s %s", LogLevelStrings[level].c_str(), message];
    NSLog(logMsg);
}
