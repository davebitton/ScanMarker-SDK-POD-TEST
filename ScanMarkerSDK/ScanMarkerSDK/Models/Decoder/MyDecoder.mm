//
//  MyDecoder.m
//  ScanMarker
//
//  Created by Sion Sasson on 6/30/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import "MyDecoder.h"
#import "SMProcess.h"

@implementation MyDecoder

@synthesize width, height, raw, defaultHeight, defaultWidth;

- (instancetype)init
{
    self = [super init];
    if (self) {
        defaultWidth = 15000;
        defaultHeight = 128;
        width = 15000;
        height = 128;
        
        raw = (uint8_t *)malloc(width * height);
        memset(raw, 0, width * height);
    }
    return self;
}

- (void)newDecode:(char *)filename Destination:(char *)destFilename rightHand:(BOOL)isRight {
    JBig85Expand(filename, destFilename);
    
    GetImageFromFile(destFilename, raw, width, height);
    
    if (!isRight) {
        [self flipImageHorizontally];
    }

    NSLog(@"W: %i  H: %i", width, height);
}

- (void)decode:(const char *)filename RightHand:(BOOL)isRight {
    
    //long timestamp = [[NSDate date] timeIntervalSince1970];
    //NSLog(@"Starting decoding (%s) at %lu", filename, timestamp);
          
    FILE* fff = fopen(filename, "rb");
	if (fff == NULL) return;
    
    int widthIn = width;
    
    uint8_t * dataWithCorrection = (uint8_t *)malloc(widthIn * height);
    memset(dataWithCorrection, 0, widthIn * height);
    
    uint8_t * data = (uint8_t *)malloc(widthIn * height);
    memset(data, 0, widthIn * height);
    
    for ( int i = 0; i < widthIn; i++)
    {
        for ( int j = 0; j < height; j++)
        {
            dataWithCorrection[j * widthIn + i] = 0;
            data[j * widthIn + i] = 0;
        }
    }
    
    int filled = 0;
    
    while (true) {
        if (!fread(data + filled, 1, 1, fff))
            break;
        filled++;
    }
    
    fclose(fff);
    int dataSize = filled * 8;
    
    for ( int i = 0; i < widthIn; i++) {
        for ( int j = 0; j < height; j++) {
            raw[j * widthIn + i] = 0;
        }
    }
    
    int indColumn = 0;
    int indBit = 0;
    int iData = 0;
    
    while ( true) {
		if (iData >= dataSize) break;
		int iDataByte = iData / 8;
		int iDataBit = iData % 8;
		bool isNewColumn = (data[iDataByte] & ( 0x80 >> iDataBit)) == 0;
		iData++;
		if (isNewColumn) {
            indBit+= 32;
            indColumn = indBit / 128;;
            if (indColumn >= widthIn - 1) // overflow of pRawData output array and data local array.
                break;

            continue;
        }
        
        uint8_t indexPattern = 0;
        uint8_t indexPattern1 = 0;
        uint8_t lengthPattern = 0;
        for ( int c = 0; c < 5; c++) {
            iDataByte = iData / 8;
            iDataBit = iData % 8;
            uint8_t bit = (data[iDataByte] & (0x80 >> iDataBit)) ? 0x80 : 0;
            indexPattern1 |= bit >> (c+3);
            iData++;
        }
        int endOfPatternBit = 0;
        while (!endOfPatternBit) {
            lengthPattern++;
            iDataByte = iData / 8;
            iDataBit = iData % 8;
            endOfPatternBit = data[iDataByte] & (0x80 >> iDataBit);
            iData++;
            if (iData >= dataSize) break;
        }
        indexPattern = indexPattern1 + (indBit % 128);
        for ( int j = indexPattern, c = 0; c < lengthPattern; c++, j++) {
            raw[j * widthIn + indColumn] = 255;
        }
    }
    
    int widthOut = indColumn + 1;
    
    for ( int i = 0; i < widthOut; i++)
	{
		for ( int j = 0; j < height; j++)
		{
			data[j*widthOut+i] = raw[j*widthIn+i];
		}
	}
    
	for ( int i = 0; i < widthOut; i++)
	{
		for ( int j = 0; j < height; j++)
		{
			raw[j * widthOut * 3 + i * 3] = 0;
            raw[j * widthOut * 3 + i * 3 + 1] = 0;
            raw[j * widthOut * 3 + i * 3 + 2] = 0;
		}
	}
    
	for ( int j = 0; j < height; j++)
	{
		int i = 0;
        raw[j * widthOut + i] = data[j*widthOut+i];
	}
	for ( int i = 1; i < widthOut; i++)
	{
		for ( int j = 0; j < height; j++)
		{
			if (data[j*widthOut+i] == 255)
			{
				raw[j * widthOut + i] = 255 - raw[j*widthOut + (i-1)];
            }
			else
			{
				raw[j*widthOut + i ] = raw[j * widthOut + (i - 1)];
		    }
		}
	}
    
    width = widthOut;
    
    if (!isRight) {
        [self flipImageHorizontally];
    }
    
    int numberOfBlacks = 0;
    int numberOfWhites = 0;
    for (int x = 0; x < width * height; x++) {
        if (raw[x] == 0) numberOfBlacks++;
        else numberOfWhites++;
    }
    
    if (numberOfBlacks > numberOfWhites) {
        NSLog(@"White text(%i). Black background(%i)", numberOfWhites, numberOfBlacks);
        
        for (int x = 0; x < width * height; x++)
            raw[x] = 255 - raw[x];
    }
    else {
        NSLog(@"Black text(%i). White background(%i)", numberOfBlacks, numberOfWhites);
    }
    
    free(dataWithCorrection);
    free(data);
}

- (void)flipImageHorizontally {
    NSLog(@"Left hand scan. Flipping image");
    
    uint8_t *rawCopy = (uint8_t *)malloc(width * height);
    
    memcpy(rawCopy, raw, width * height);
    
    for ( int i = 0; i < width; i++)
    {
        for ( int j = 0; j < height; j++)
        {
            raw[(height - j - 1) * width + (width - i - 1)] = rawCopy[j * width + i];
        }
    }
}

- (void)dealloc {
    free(raw);
}

@end
