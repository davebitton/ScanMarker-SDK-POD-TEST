//
//  PlatformDefine.h
//  licensing
//
//  Created by Yury Markman on 13/10/2018.
//  Copyright © 2018 Scanmarker. All rights reserved.
//

#ifndef __PLATFORMDEFINE_H__
#define __PLATFORMDEFINE_H__

#ifdef __APPLE__

#ifdef LICENSE_IOS
#define USE_MACHINE_ID_CODE 0

#else
#define USE_MACHINE_ID_CODE 1
#endif//TARGET_OS_MAC

#else
#ifdef __ANDROID__
#define USE_MACHINE_ID_CODE 2
#else//windows
#define USE_MACHINE_ID_CODE 1

#endif//__ANDROID__
#endif//__APPLE__

#if (USE_MACHINE_ID_CODE==1)

extern bool validateFingerprint(std::string &testString);

#include "MachineUniqueId.h"
#include <vector>
#endif//USE_MACHINE_ID_CODE

#if defined(WIN32) || defined(WIN64)
#include "dirent.h"
// Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)

static const char *SLASH = "\\";

#else
#include <dirent.h>
#include <sys/stat.h>
static const char * SLASH = "/";
#endif

#endif /* __PLATFORMDEFINE_H__ */
