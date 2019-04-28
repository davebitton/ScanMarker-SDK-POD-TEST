//
//  HebrewDllLibrary.m
//  HebrewDllLibrary
//
//  Created by Sion Sasson on 7/21/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import "HebrewDllLibrary.h"

#import "HebrewProductionDll.h"
#import "xvbmp.h"

@implementation HebrewDllLibrary

- (void)processWithInput:(uint8_t *)input w_In:(int)w_In h_In:(int)h_In inputRaw:(uint8_t *)inputRaw w_Raw:(int)w_Raw h_Raw:(int)h_Raw output_OCR:(uint8_t *)output_OCR w_OCR:(int*)w_OCR h_OCR:(int*)h_OCR output_IC:(uint8_t *)output_IC w_IC:(int)w_IC h_IC:(int)h_IC italicStep:(int)italicStep debugMode:(int)debugMode debugFileName:(char*)debugFileName langusageCode:(int)langCode {
    
    int w_OCR1 = *w_OCR;
    int h_OCR1 = *h_OCR;
    
    ProcessImage(input, w_In, h_In, inputRaw, w_Raw, h_Raw, output_OCR, w_OCR1, h_OCR1, output_IC, w_IC, h_IC, italicStep, debugMode, debugFileName, NULL, langCode);
    
    *w_OCR = w_OCR1;
    *h_OCR = h_OCR1;
    
}

+ (void)writeBMPToPath:(NSString*)path Data:(uint8_t *)data Witdh:(int)width Height:(int)height {
    
    FILE * fp = fopen([path UTF8String], "wb");
    
    if (fp) {
        WriteBMP(fp, data, PIC24, width, height, NULL, NULL, NULL, 8, F_FULLCOLOR);
        fclose(fp);
    }
}

@end
