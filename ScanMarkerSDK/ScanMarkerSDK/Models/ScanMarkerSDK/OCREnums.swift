//
//  OCREnums.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 17/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

extension ScanMarkerSDK {
    //OCR direction
    public enum OCRDirection : String, CaseIterable {
        case left, right
    }
    
    public enum OCRMode {
        case online, offline
    }
    
    public enum ScanningType : String, CaseIterable{
        case text, image
    }
}
