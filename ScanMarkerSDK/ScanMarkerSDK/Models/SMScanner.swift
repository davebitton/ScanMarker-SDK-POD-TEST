//
//  SMScanner.swift
//  ScanMarkerSDK_Testing
//
//  Created by dave bitton on 06/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation
import CoreBluetooth

public class SMScanner {
    
    fileprivate(set) var peripheral : CBPeripheral!
    fileprivate(set) var deviceVersion : Int?
    fileprivate(set) var firmwareVersion : Float?
    fileprivate(set) var firmwareData : Data?
    fileprivate(set) var batteryPercentage : Int?
    fileprivate(set) var serialNumber : String?
    
    internal func setFirmwareVersion(_ value : Float?){
        self.firmwareVersion = value
    }
    
    internal func setFirmwareData(_ value : Data?){
        self.firmwareData = value
    }
    
    internal func setBatteryPercentage(_ value : Int?){
        self.batteryPercentage = value
    }
    
    internal func setDeviceVersion(_ value : Int?){
        self.deviceVersion = value
    }
    
    internal func setSerialNumber(_ value : String?){
        self.serialNumber = value
    }
    
    public init(_ peripheral : CBPeripheral) {
        self.peripheral = peripheral
    }
    
    public func getName() -> String  {
        return self.peripheral.name ?? "SMScanner"
    }
    
}

public struct SMScannerStatus {
    var firmwareData : Data?
    var firmwareVersion : Float?
    var batteryPercentage : Int?
    var isCharging : Bool?
}
