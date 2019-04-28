//
//  BluetoothManager.swift
//  ScanMarkerSDK_Testing
//
//  Created by dave bitton on 04/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//


import Foundation
import CoreBluetooth
import ScanMarkerSDKPrivate


class BluetoothManager : NSObject {
    
    fileprivate var rxCharacteristic : CBCharacteristic!
    fileprivate var txCharacteristic : CBCharacteristic!
    fileprivate var centralManager: CBCentralManager!
    fileprivate var activeScanner: SMScanner!
    fileprivate var discoveredScanners = [SMScanner]()
    fileprivate let timer = 5 //TODO: initiate with real timer logic in discoverPeriphelas function
    fileprivate let dataHandler = BLEDataHandler()
    fileprivate let ocrManager = OCRManager()
    
    weak var delegate : ScanMarkerSDKBluetoothDelegate?
    weak var scannerDelegate : ScanMarkerSDKScannerDelegate?
    
    override init() {
        super.init()
        dataHandler.delegate = self
    }
    
    //MARK: - Public methods
    
    func checkState(delegate : ScanMarkerSDKBluetoothDelegate?){
        self.delegate = delegate
        centralManager = CBCentralManager(delegate: self, queue: nil)
    }
    
    func connectTo(peripheral : CBPeripheral, delegate : ScanMarkerSDKScannerDelegate?){
        self.scannerDelegate = delegate
        peripheral.delegate = self
        centralManager.connect(peripheral, options: nil)
    }
}

//MARK: - Protocol CBCentralManagerDelegate functions
extension BluetoothManager :  CBCentralManagerDelegate {
    
    internal func centralManagerDidUpdateState(_ central: CBCentralManager) {
        self.delegate?.bluetoothDidUpdateState(state: central.state)
        switch central.state {
        case .unknown:
            print("central.state is .unknown")
        case .resetting:
            print("central.state is .resetting")
        case .unsupported:
            print("central.state is .unsupported")
        case .unauthorized:
            print("central.state is .unauthorized")
        case .poweredOff:
            print("central.state is .poweredOff")
        case .poweredOn:
            centralManager.scanForPeripherals(withServices: [urtServiceCBUUID], options: nil)
            print("central.state is .poweredOn")
        default:
            print("error")
        }
    }
    
    internal func centralManager(_ central: CBCentralManager, didDiscover peripheral: CBPeripheral,
                                 advertisementData: [String : Any], rssi RSSI: NSNumber) {
        print(peripheral)
        discoveredScanners.append(SMScanner(peripheral))
        if timer >= 5 {
            centralManager.stopScan()
            self.delegate?.didDiscover(scanners: self.discoveredScanners)
        }
    }
    
    internal func centralManager(_ central: CBCentralManager, didConnect peripheral: CBPeripheral) {
        activeScanner = SMScanner(peripheral)
        activeScanner.peripheral.discoverServices([urtServiceCBUUID, deviceInformationServiceCBUUID])
        if !ScanMarkerSDK.sharedInstance.shouldCheckLicense {
            //incase of license activation is disabled so notify that connection has done
            self.scannerDelegate?.scannerDidConnect(scanner: activeScanner)
        }
    }
    
    internal func centralManager(_ central: CBCentralManager, didFailToConnect peripheral: CBPeripheral, error: Error?) {
        self.scannerDelegate?.scannerConnectionFailedWith(scanner: SMScanner(peripheral), error: .unknown)
    }
    
}

extension BluetoothManager : CBPeripheralDelegate {
    
    internal func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services else {return}
        services.forEach { (service) in
            if service.uuid == urtServiceCBUUID {
                //It is a UART service *(documented in constants)
                //so discover the transmitter and reciever charateristics
                peripheral.discoverCharacteristics([rxCharacteristicCBUUID, txCharacteristicCBUUID], for: service)
            }else if service.uuid == deviceInformationServiceCBUUID {
                //It is the device information service
                //so discover the hardware revision
                peripheral.discoverCharacteristics([hardwareRevisionCBUUID], for: service)
            }
        }
    }
    
   internal func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let characteristics = service.characteristics else { return }
        print("service: ", service)
        for characteristic in characteristics {
            print("characteristic:", characteristic)
            if characteristic.uuid == rxCharacteristicCBUUID {
                self.rxCharacteristic = characteristic
                peripheral.setNotifyValue(true, for: characteristic)
            }
            else if characteristic.uuid == txCharacteristicCBUUID {
                self.txCharacteristic = characteristic
            }
            else if characteristic.uuid == hardwareRevisionCBUUID {
                peripheral.readValue(for: characteristic)
            }
        }
    }
    
    internal func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        print(characteristic)
        if let value = characteristic.value, characteristic == rxCharacteristic {
             self.dataHandler.parse(data: value, from: activeScanner , delegate: self)
        }else if characteristic.uuid == hardwareRevisionCBUUID {
            //extract hardware revision with characteristic.value
            print(characteristic.value ?? "no data")
        }
    }
    
    internal func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        
    }
}

extension BluetoothManager : BLEDataHandlerDelegate {
    
    internal func parsingDidFailed(dataType: BLEDataHandler.DataType, error: SDKScanError) {
        switch dataType {
        case .scanningData:
            self.scannerDelegate?.scanningFailedWith(error: error, scanner: activeScanner)
        default:
            break
        }
    }
    
    
    internal func didParsed(dataType: BLEDataHandler.DataType, value: Any?) {
        switch dataType {
        case .scannerStatus:
            guard let status = value as? SMScannerStatus else {return}
            activeScanner.setFirmwareVersion(status.firmwareVersion)
            activeScanner.setFirmwareData(status.firmwareData)
            activeScanner.setBatteryPercentage(status.batteryPercentage)
            //reading here the serial number because the response from reading serial number start with the firmware version
            //this way, once we know the firmware version, we can identify the result from reading the serial number
            if ScanMarkerSDK.sharedInstance.shouldCheckLicense, activeScanner.serialNumber == nil {
                readSerialNumber(fromScanner: activeScanner)
            }else{
                self.scannerDelegate?.scannerStatusDidUpdate(activeScanner, status: status)
            }
        case .serialNumber:
            if let SN = value as? String {
                activeScanner.setSerialNumber(SN)
                if let error = activateLicenseIfNeeded(){
                    self.scannerDelegate?.scannerConnectionFailedWith(scanner: activeScanner, error: error)
                }else{
                     self.scannerDelegate?.scannerDidConnect(scanner: activeScanner)
                }
            }
            break
        case .scannerVersion:
            guard let version = value as? Int else {return}
            //scanner version is 1 or 2 and compressed version should be 0 or 1. 
            ScanMarkerSDK.sharedInstance.setCompressedVersion(version - 1)
        case .scanningData:
            break
        case .startOfScanningData:
            self.scannerDelegate?.didStartScanning(activeScanner)
            break
        case .endOfScanningData:
            guard let scanningData = value as? Data else {return}
            self.scannerDelegate?.didStartProcessingScanning(activeScanner)
            ocrManager.process(data: scanningData, fromScanner: activeScanner, listener: self.scannerDelegate)
        }
    }
}

//MARK: - Private methods
extension BluetoothManager {
    
    fileprivate func readSerialNumber(fromScanner scanner : SMScanner){
        //write dummy command because of a technical bug
        let dummyDataCommand = Data(hex: "0A")
        scanner.peripheral.writeValue(dummyDataCommand, for: self.txCharacteristic, type: .withResponse)
        //write the firmware with serial number command
        let serialNumberDataCommand = Data(hex: "22")
        scanner.peripheral.writeValue(serialNumberDataCommand, for: self.txCharacteristic, type: .withResponse)
    }
    
    //function returns an error if activation failed, if success returns nil
    fileprivate func activateLicenseIfNeeded() -> SDKError? {
        let isLicenseExist = ScanMarkerLicensing().checkLicense(activeScanner.serialNumber, ScanMarkerSDK.sharedInstance.deviceToken)
        if isLicenseExist {
            return nil
        }else{
            //try to acquire license
            let result = ScanMarkerLicensing().acquireLicense(activeScanner.serialNumber, ScanMarkerSDK.sharedInstance.deviceToken)
            return parseActivationLicenseResult(Int(result))
        }
    }
    
    //returns a SDK error according to acquireLicense result
    //returns nil if acquiring succeed
    fileprivate func parseActivationLicenseResult(_ result : Int) -> SDKError?{
        switch result {
        case 0: //Activation success
            return nil
        case -2: //to many licenses
            return .tooManyLicenses
        default:
            return .failedToAcquireLicense
        }
    }
}
