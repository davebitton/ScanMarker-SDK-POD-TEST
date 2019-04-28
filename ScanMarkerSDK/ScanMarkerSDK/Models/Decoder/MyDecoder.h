//
//  MyDecoder.h
//  ScanMarker
//
//  Created by Sion Sasson on 6/30/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MyDecoder : NSObject {
}

@property (assign) int width;
@property (assign) int height;
@property (assign) uint8_t * raw;

@property (assign) int defaultWidth;
@property (assign) int defaultHeight;

- (void)decode:(const char *)filename RightHand:(BOOL)isRight;
- (void)newDecode:(char *)filename Destination:(char * )destFilename rightHand:(BOOL)isRight;

@end
