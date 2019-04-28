//
//  BLEDataHandler.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 11/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation
import ScanMarkerSDKPrivate

protocol BLEDataHandlerDelegate : class {
    func didParsed(dataType : BLEDataHandler.DataType, value : Any?)
    func parsingDidFailed(dataType : BLEDataHandler.DataType, error : SDKScanError)
}


class BLEDataHandler {
    
    //Hex string represents the start / end of battery data
    fileprivate let endBatteryV1 = "ffffff05"
    fileprivate let endBatteryV2 = "ffffff0405"
    fileprivate let startBatteryV1 = "ffffff02"
    fileprivate let startBatteryV2 = "ffffff0402"
    
    //BTD - bluetooth scanning data. Hex string represents the start / end of blutooth scanning data
    fileprivate let endBTDV1 = "ffffff07"
    fileprivate let endBTDV2 = "ffffff0407"
    fileprivate let startBTDV1 = "ffffff00"
    fileprivate let startBTDV2 = "ffffff0400"
    
    //different data ranges depends on scanner transmitting data version
    fileprivate var dataRangesV1 : DataRanges!
    fileprivate var dataRangesV2 : DataRanges!
    fileprivate var suffixesV1 = [Data]()
    fileprivate var suffixesV2 = [Data]()
    
    //currentData being recived by current scanner being connected
    fileprivate var currentData : Data!
    fileprivate var currentScanner : SMScanner!
    fileprivate var currentDataRange : DataRanges!
    
    //data ranges struct will hold 4 ranges
     //BTD - bluetooth scanning data. Hex string represents the start / end of blutooth data
    struct DataRanges{
        var startBattery : Range<Data.Index>?
        var endBattery : Range<Data.Index>?
        var startBTD : Range<Data.Index>?
        var endBTD : Range<Data.Index>?
    }
    
    enum DataType {
        case serialNumber
        case scannerStatus
        case scannerVersion
        case scanningData
        case endOfScanningData
        case startOfScanningData
    }
    
    //All the data being recived by current scanner being connected
    //It will be cleared after end of bluetooth data has parsed
    fileprivate var allDataFromSingleScan : Data?
    
    fileprivate var suspectedEndOfData : Data?
    
    //BLEData delegate
    weak var delegate : BLEDataHandlerDelegate?
    
    init() {
        initSuffixes()
    }
    
    //public methods
    
    func parse(data: Data, from: SMScanner ,delegate : BLEDataHandlerDelegate?){
        self.delegate = delegate
        self.currentData = data
        self.currentScanner = from
        self.initRanges()
        if currentScanner.deviceVersion == nil {
            if let version = identifyScannerVersion() {
                currentScanner.setDeviceVersion(version)
                self.delegate?.didParsed(dataType: .scannerVersion, value: version)
            }else{
                self.delegate?.parsingDidFailed(dataType: .scannerVersion, error: .invalidScannerVersion)
                return
            }
        }
        self.currentDataRange = currentScanner.deviceVersion == 1 ? dataRangesV1 : dataRangesV2
        self.startParsing()
    }
}

extension BLEDataHandler {
    
    //private methods
    
    fileprivate func initRanges(){
        self.dataRangesV1 = DataRanges(startBattery: currentData?.range(of: Data(hex: startBatteryV1), options: .backwards, in: nil),
                                       endBattery: currentData?.range(of: Data(hex: endBatteryV1), options: .backwards, in: nil),
                                       startBTD: currentData?.range(of: Data(hex: startBTDV1), options: .backwards, in: nil),
                                       endBTD: currentData?.range(of: Data(hex: endBTDV1), options: .backwards, in: nil))
        
        self.dataRangesV2 = DataRanges(startBattery: currentData?.range(of: Data(hex: startBatteryV2), options: .backwards, in: nil),
                                       endBattery: currentData?.range(of: Data(hex: endBatteryV2), options: .backwards, in: nil),
                                       startBTD: currentData?.range(of: Data(hex: startBTDV2), options: .backwards, in: nil),
                                       endBTD: currentData?.range(of: Data(hex: endBTDV2), options: .backwards, in: nil))
    }
    
    fileprivate func initSuffixes(){
        let endDataV1 = Data(hex: endBTDV1)
        let endDataV2 = Data(hex: endBTDV2)
        for index in 0...endDataV1.count - 1 {
            suffixesV1.append(endDataV1.subdata(in: 0...index + 1))
        }
        for index in 0...endDataV1.count - 1 {
            suffixesV2.append(endDataV2.subdata(in: 0...index + 1))
        }
    }
    
    fileprivate func startParsing(){
        guard let version = currentScanner?.deviceVersion else{ return}
        let suspectDidFound = handleSuffixesIfNeeded(version)
        if currentDataRange.startBattery != nil && currentDataRange.endBattery != nil {
            handleBatteryAndFirmwareData()
        }else{
            //incase of serial number data
            //serial number data starts with firmware data and then the serial number data
            if let firmwareData = currentScanner.firmwareData,
                let firmwareRange = currentData.range(of: firmwareData),
                firmwareRange.lowerBound == 0{ //incase of firmware data was on start
                let serialNumberData = currentData.subdata(in: firmwareRange.upperBound...currentData.count)
                let serialNumberString = handleSerialNumberData(serialNumberData)
                self.delegate?.didParsed(dataType: .serialNumber, value: serialNumberString)
            }else{
                if ScanMarkerSDK.sharedInstance.shouldCheckLicense {
                    guard ScanMarkerLicensing().checkLicense(currentScanner.serialNumber, ScanMarkerSDK.sharedInstance.deviceToken) else {
                        self.delegate?.parsingDidFailed(dataType: .scanningData, error: .noLicense)
                        return
                    }
                }
            }
        }
        
        //incase there is data in self.suspectedEndOfData but suspect did not found on this currentData parsing
        if let suspectedEndOfData = self.suspectedEndOfData, !suspectDidFound {
            self.suspectedEndOfData?.append(currentData)
            self.currentDataRange.endBTD = suspectedEndOfData.range(of: Data(hex: version == 1 ? endBTDV1 : endBTDV2), options: .backwards, in: nil)
            self.suspectedEndOfData = nil;
        }
        
        //incase of end of scanning
        if (currentDataRange.endBTD != nil) {
            handleEndOfScanningData()
        }
        
        //incase of start of scanning
        if (currentDataRange.startBTD != nil){
            handleStartOfScanningData()
        }
        
        //incase of in the middle of scanning
        if (currentDataRange.startBTD == nil && currentDataRange.endBTD == nil) {
            if self.allDataFromSingleScan != nil {
                self.allDataFromSingleScan?.append(self.currentData)
            }
        }
    }
    
    //find suspect of bluetooth end data
    fileprivate func handleSuffixesIfNeeded(_ version : Int) -> Bool{
        let suffixes = version == 1 ? suffixesV1 : suffixesV2
        if self.suspectedEndOfData == nil,
            let _ = self.allDataFromSingleScan,
            let endData = currentDataRange.endBTD,
            currentData.count > endData.count {
            let lastData = currentData.subdata(in: (currentData.count - endData.count)...currentData.count)
            for index in stride(from: suffixes.count, through: 1, by: -1) {
                let suffix = suffixes[index - 1]
                let dataToCompare = NSData(data: lastData.subdata(in: (lastData.count - suffix.count)...lastData.count))
                if dataToCompare.isEqual(to: suffix) {
                    self.suspectedEndOfData = dataToCompare.subdata(with: NSRange(location: 0, length: dataToCompare.count))
                    return true
                }
            }
        }
        return false
    }
    
}

//MARK :- Parsing private methods
extension BLEDataHandler {
    
    //could be 1 or 2
    fileprivate func identifyScannerVersion() -> Int?{
        if (dataRangesV1.startBattery != nil  && dataRangesV1.endBattery != nil) {
            return 1
        }
        else if (dataRangesV2.startBattery != nil && dataRangesV2.endBattery != nil) {
            return 2
        }
        
        if dataRangesV2.startBTD != nil {
            return 2
        }else if dataRangesV1.startBTD != nil {
            return 1
        }
        return nil
    }
    
    
    fileprivate func handleBatteryAndFirmwareData(){
        //TODO: parse battery data from currentData
        guard let startBatteryIndex = self.currentDataRange.startBattery?.upperBound else {return}
        guard let endBatteryIndex = self.currentDataRange.endBattery?.lowerBound else {return}
        let batteryDataWithFirmware = self.currentData.subdata(in: startBatteryIndex...endBatteryIndex)
        let firmwareData = batteryDataWithFirmware.subdata(in: 0...(batteryDataWithFirmware.count / 2))
        let batteryData = batteryDataWithFirmware.subdata(in: (batteryDataWithFirmware.count / 2)...batteryDataWithFirmware.count)
        self.delegate?.didParsed(dataType: .scannerStatus,
                                 value: SMScannerStatus(firmwareData: firmwareData,
                                                                                  firmwareVersion: handleFirmwareData(firmwareData),
                                                                                  batteryPercentage: handleBatteryPercentage(batteryData),
                                                                                  isCharging: handleBatteryCharging(batteryData) ))
        //TODO:
        // - handle charging state from battery data
    }
    
    fileprivate func handleFirmwareData(_ firmwareData : Data) -> Float?{
        let result = hexStringToInt(firmwareData.hexDescription)
        let firmwareString = String(format: "%d.%02d", result >> 8 , result & 0xff)
        return Float(firmwareString)
    }
    
    fileprivate func handleBatteryPercentage(_ batteryData : Data) -> Int?{
        let batteryPercentageString = batteryData.hexDescription.substring(with: 2..<4)
        return Int(hexStringToInt(batteryPercentageString))
    }
    
    fileprivate func handleBatteryCharging(_ batteryData : Data) -> Bool?{
        let subHexFromIndex1 = batteryData.hexDescription.substring(with: 1..<2)
        if let value = Int(subHexFromIndex1, radix: 16){
            print("value ", value)
            var binary = String(value, radix: 2)
            binary = binary.pad(toSize: 4)
            binary = binary.substring(with: 2..<binary.count)
            switch binary {
            case "01" :
                return true //battery is charging
            default:
                return false
            }
        }else{
            return nil
        }
    }
    
    fileprivate func handleSerialNumberData(_ SNData : Data) -> String?{
        return ScanMarkerLicensing().parseSerialNumber(SNData)?.description
    }
    
    fileprivate func handleEndOfScanningData(){
        guard let endDataIndex = self.currentDataRange.endBTD?.lowerBound else {return}
        self.allDataFromSingleScan?.append(self.currentData.subdata(in: 0...endDataIndex))
        self.delegate?.didParsed(dataType: .endOfScanningData, value: self.allDataFromSingleScan)
    }
    
    fileprivate func handleStartOfScanningData(){
        guard let startDataUpperIndex = self.currentDataRange.startBTD?.upperBound else {return}
        let dataStartFrom = currentScanner.deviceVersion == 1 ? startDataUpperIndex * 2 : startDataUpperIndex + 4
        if currentData.count >= dataStartFrom {
            self.allDataFromSingleScan = Data()
            self.allDataFromSingleScan?.append(currentData.subdata(in: dataStartFrom...currentData.count))
            self.delegate?.didParsed(dataType: .startOfScanningData, value: self.allDataFromSingleScan)
        }
    }
    
    fileprivate func hexStringToInt(_ hexString : String) -> UInt32{
        var result : UInt32 = 0
        _ = Scanner(string: hexString).scanHexInt32(&result)
        return result
    }
    
}
