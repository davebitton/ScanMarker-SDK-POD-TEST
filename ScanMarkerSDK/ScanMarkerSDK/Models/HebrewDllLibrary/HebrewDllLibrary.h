//
//  HebrewDllLibrary.h
//  HebrewDllLibrary
//
//  Created by Sion Sasson on 7/21/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HebrewDllLibrary : NSObject

/*
uint8_t * input;
int w_In;
int h_In;
uint8_t * inputRaw;
int w_Raw;
int h_Raw;
uint8_t * output_OCR;
int w_OCR;
int h_OCR;
uint8_t * output_IC;
int w_IC;
int h_IC;
int italicStep;
int debugMode;
char * debugFileName;

ProcessImage(input, w_In, h_In, inputRaw, w_Raw, h_Raw, output_OCR, w_OCR, h_OCR, output_IC, w_IC, h_IC, italicStep, debugMode, debugFileName);
*/

- (void)processWithInput:(uint8_t *)input w_In:(int)w_In h_In:(int)h_In inputRaw:(uint8_t *)inputRaw w_Raw:(int)w_Raw h_Raw:(int)h_Raw output_OCR:(uint8_t *)output_OCR w_OCR:(int*)w_OCR h_OCR:(int*)h_OCR output_IC:(uint8_t *)output_IC w_IC:(int)w_IC h_IC:(int)h_IC italicStep:(int)italicStep debugMode:(int)debugMode debugFileName:(char*)debugFileName langusageCode:(int)langCode;

+ (void)writeBMPToPath:(NSString*)path Data:(uint8_t *)data Witdh:(int)width Height:(int)height;

@end
