//
//  TextImageAnalyzing.h
//  TextImageAnalyzing
//
//  Created by Sion Sasson on 7/20/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TextImageAnalyzing : NSObject

@property (assign) uint8_t * resultData;

- (const char *)checkScan:(uint8_t *)rawImage ResultImage:(uint8_t *)resultImage  Width:(int)width Height:(int)height;

@end
