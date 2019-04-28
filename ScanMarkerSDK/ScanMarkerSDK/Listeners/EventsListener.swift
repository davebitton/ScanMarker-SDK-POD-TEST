//
//  EventsListener.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 11/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import CoreBluetooth

public protocol ScanMarkerSDKScannerDelegate : class {
    func scanningSuccessWith(text : String, scanner : SMScanner)
    func scanningSuccessWith(image : UIImage, scanner : SMScanner)
    func scanningFailedWith(error : SDKScanError, scanner : SMScanner)
    func scannerDidConnect(scanner : SMScanner)
    func scannerConnectionFailedWith(scanner : SMScanner, error : SDKError)
    func scannerStatusDidUpdate(_ scanner : SMScanner, status : SMScannerStatus)
    func didStartScanning(_ scanner : SMScanner)
    func didStartProcessingScanning(_ scanner : SMScanner)
}

public protocol ScanMarkerSDKBluetoothDelegate : class {
    func bluetoothDidUpdateState(state : CBManagerState)
    func didDiscover(scanners : [SMScanner])
}

extension ScanMarkerSDKScannerDelegate {
    func scannerStatusDidUpdate(_ scanner : SMScanner, status : SMScannerStatus){}
}
