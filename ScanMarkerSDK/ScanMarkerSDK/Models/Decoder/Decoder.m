//
//  Decoder.m
//  Decoder
//
//  Created by Sion Sasson on 7/20/14.
//  Copyright (c) 2014 sion sasson. All rights reserved.
//

#import "Decoder.h"
#import "MyDecoder.h"

@implementation Decoder

- (instancetype)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

- (void)newDecode:(char *)filename Destination:(char * )destFilename RightHand:(BOOL)isRight {
    self.decoder = [MyDecoder new];
    [self.decoder newDecode:filename Destination:destFilename rightHand:isRight];
}

- (void)decode:(const char *)filename RightHand:(BOOL)isRight {
    self.decoder = [MyDecoder new];
    [self.decoder decode:filename RightHand:isRight];
}

- (uint8_t *)getRawData {
    return self.decoder.raw;
}

- (int)getDataWidth {
    return self.decoder.width;
}

- (int)getDataHeight {
    return self.decoder.height;
}

- (int)getDefaultWidth {
    return self.decoder.defaultWidth;
}

- (int)getDefaultHeight {
    return self.decoder.defaultHeight;
}

@end
