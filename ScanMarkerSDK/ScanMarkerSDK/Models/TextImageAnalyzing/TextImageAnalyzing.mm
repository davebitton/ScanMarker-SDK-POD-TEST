//
//  TextImageAnalyzing.m
//  TextImageAnalyzing
//
//  Created by Sion Sasson on 7/20/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import "TextImageAnalyzing.h"

#import "TextImageAnalyzingDll.h"

@implementation TextImageAnalyzing

@synthesize resultData;

- (const char *)checkScan:(uint8_t *)rawImage ResultImage:(uint8_t *)resultImage  Width:(int)width Height:(int)height {
    return CheckScan(false, 220, 1, width, height, rawImage, resultImage, NULL, 0, false);
}

@end
