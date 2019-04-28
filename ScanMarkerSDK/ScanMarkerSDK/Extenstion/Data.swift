//
//  Data.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 13/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation

extension Data {
    init(hex:String) {
        let scalars = hex.unicodeScalars
        var bytes = Array<UInt8>(repeating: 0, count: (scalars.count + 1) >> 1)
        for (index, scalar) in scalars.enumerated() {
            var nibble = scalar.hexNibble
            if index & 1 == 0 {
                nibble <<= 4
            }
            bytes[index >> 1] |= nibble
        }
        self = Data(bytes: bytes)
    }
    
    func subdata(in range: ClosedRange<Index>) -> Data {
        return subdata(in: range.lowerBound ..< range.upperBound)
    }
    
    var hexDescription: String {
            return reduce("") {$0 + String(format: "%02x", $1)}
    }
    
    func writeTo(url: URL, atomically : Bool) -> Bool{
        return NSData(data: self).write(to: url, atomically: atomically)
    }
    
}

extension UnicodeScalar {
    var hexNibble:UInt8 {
        let value = self.value
        if 48 <= value && value <= 57 {
            return UInt8(value - 48)
        }
        else if 65 <= value && value <= 70 {
            return UInt8(value - 55)
        }
        else if 97 <= value && value <= 102 {
            return UInt8(value - 87)
        }
        fatalError("\(self) not a legal hex nibble")
    }
}
