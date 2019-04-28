//
//  ImageChecker.h
//  TextImageAnalyzing
//
//  Created by Sion Sasson on 7/3/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ImageChecker : NSObject

+ (char *)checkBMP:(BOOL)isBkgrWhite Language:(int)language ImageType:(int)imageType Width:(int)width Height:(int)height Image:(uint8_t *)image ImageOut:(uint8_t *)imageOut OutpuSnakes:(int *)outputSnakes MaxSnakes:(int)maxNumberOutputSnakes IsVertical:(BOOL)isChineseVertical;

@end
