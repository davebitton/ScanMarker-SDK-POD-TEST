//
//  ImageChecker.m
//  TextImageAnalyzing
//
//  Created by Sion Sasson on 7/3/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import "ImageChecker.h"
#import "TextImageAnalyzingDll.h"

@implementation ImageChecker

+ (char *)checkBMP:(BOOL)isBkgrWhite Language:(int)language ImageType:(int)imageType Width:(int)width Height:(int)height Image:(uint8_t *)image ImageOut:(uint8_t *)imageOut OutpuSnakes:(int *)outputSnakes MaxSnakes:(int)maxNumberOutputSnakes IsVertical:(BOOL)isChineseVertical {
    return CheckScan(isBkgrWhite, language, imageType, width, height, image, imageOut, outputSnakes, maxNumberOutputSnakes, false);
}

@end
