//
//  Decoder.h
//  Decoder
//
//  Created by Sion Sasson on 7/20/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MyDecoder;

@interface Decoder : NSObject

@property (nonatomic, strong) MyDecoder * decoder;

- (void)decode:(const char *)filename RightHand:(BOOL)isRight;
- (void)newDecode:(char *)filename Destination:(char * )destFilename RightHand:(BOOL)isRight;

- (uint8_t *)getRawData;
- (int)getDataWidth;
- (int)getDataHeight;
- (int)getDefaultWidth;
- (int)getDefaultHeight;

@end
