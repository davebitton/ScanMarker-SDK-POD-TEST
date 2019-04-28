//
//  ScanMarkerSDK.swift
//  ScanMarkerSDK_Testing
//
//  Created by dave bitton on 06/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation

public class ScanMarkerSDK {
    
    // MARK: - Shared Intance Property
    
    public static var sharedInstance: ScanMarkerSDK = {
        let sharedInstance = ScanMarkerSDK(baseURL: "somthing")
        
        // Configuration∫
        // ...
        
        return sharedInstance
    }()
    
    //Public Configuration struct
    public struct Configuration {
        public var ocrLanguage : OCRLanguage
        public var ocrMode : OCRMode
        public var direction : OCRDirection
        public var scanningType : ScanningType
        
        public init(ocrLanguage : OCRLanguage, ocrMode: OCRMode, direction : OCRDirection , scanningType:ScanningType){
            self.ocrLanguage = ocrLanguage
            self.ocrMode = ocrMode
            self.direction = direction
            self.scanningType = scanningType
        }
    }
    
    
    //internal setter
    internal func setCompressedVersion(_ compressedVersion : Int){
        self.configuration.compressedVersion = compressedVersion
    }
    
    //Internal Configuration struct
    struct OCRConfiguration {
        var sdkConfiguration : Configuration!
        let isVerticalScanning : Bool
        var compressedVersion : Int
    }
    
    // MARK: - Properties
    let baseURL: String
    
    // MARK: - Private Properties
    private(set) var btListener: ScanMarkerSDKBluetoothDelegate?
    private(set) var scannerListener: ScanMarkerSDKScannerDelegate?
    //Flag will indicate whether to perform license validation
    private(set) var shouldCheckLicense = false
    //Configuration contains params for OCR
    private(set) var configuration = OCRConfiguration(sdkConfiguration: Configuration(ocrLanguage: .English,
                                                   ocrMode: .online,
                                                   direction: .right,
                                                   scanningType: .text),
                                                   isVerticalScanning: false,
                                                   compressedVersion: 1
                                                   )
    //Will hold the device token (iphone,ipad...)
    private(set) var deviceToken : String? = nil
    private var bluetoothManager = BluetoothManager()
    
    
    // Initialization
    
    private init(baseURL: String) {
        self.baseURL = baseURL
        self.btListener = nil
        self.scannerListener = nil
    }
    
    
    // MARK: - Accessors
    
    public func discoverScanners(listener : ScanMarkerSDKBluetoothDelegate? = nil){
        self.bluetoothManager.checkState(delegate: listener ?? self.btListener)
    }
    
    public func connectTo(scanner : SMScanner, listener : ScanMarkerSDKScannerDelegate?){
        self.scannerListener = listener
        if self.shouldCheckLicense {
            guard let _ = deviceToken else {
                self.scannerListener?.scannerConnectionFailedWith(scanner: scanner, error: .noDeviceToken)
                return
            }
        }
        self.bluetoothManager.connectTo(peripheral: scanner.peripheral, delegate: listener)
    }
    
    public func configure(listener : ScanMarkerSDKBluetoothDelegate, configuration : Configuration){
        self.btListener = listener
        self.configuration = OCRConfiguration(sdkConfiguration: configuration,
                                              isVerticalScanning: configuration.ocrLanguage.isVerticalLanguage(),
                                              compressedVersion: 1)
    }
    
    //MARK: - Setters
    public func setOcrLanguage(_ ocrLanguage : OCRLanguage){
        self.configuration.sdkConfiguration.ocrLanguage = ocrLanguage
    }
    
    public func setOcrMode(_ ocrMode : OCRMode){
        self.configuration.sdkConfiguration.ocrMode = ocrMode
    }
    
    public func setScanningType(_ scanningType : ScanningType){
        self.configuration.sdkConfiguration.scanningType = scanningType
    }
    
    public func setOcrDirection(_ direction : OCRDirection){
        self.configuration.sdkConfiguration.direction = direction
    }
    
    public func enableLicenseActivation(_ enable : Bool){
        self.shouldCheckLicense = enable
    }
    
    public func setDeviceToken(token : String){
        self.deviceToken = token
    }
}

//errors structure
public enum SDKError : String {
    case noDeviceToken
    case unknown
    case tooManyLicenses
    case failedToAcquireLicense
}

public enum SDKScanError : String {
    case scanningToSlow
    case scanningIsNotStraight
    case invalidScannerVersion
    case noLicense
    case invalidToken
    case unknown
}

