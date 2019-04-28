//
//  OCRManager.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 16/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation
import ScanMarkerSDKPrivate

class OCRManager {
    
    fileprivate var readyScanObjects = [ScanObject]()
    fileprivate var listener : ScanMarkerSDKScannerDelegate?
    
    init() {
        buildDirectoryStructure()
    }
    
    //Public method
    func process(data : Data, fromScanner scanner : SMScanner, listener : ScanMarkerSDKScannerDelegate?){
        if ScanMarkerSDK.sharedInstance.configuration.sdkConfiguration.ocrMode == .offline {
            createOCREngineIfNeeded()
        }
        self.listener = listener
        let scanObject = ScanObject(data: data, fromScanner: scanner)
        readyScanObjects.append(scanObject)
        scanObject.doOCR(delegate: self, withImageProcess: ScanMarkerSDK.sharedInstance.configuration.sdkConfiguration.scanningType == .image)
    }
    
}



//Private method
extension OCRManager {
    //Private method
    fileprivate func buildDirectoryStructure(){
        buildFolderIfNeeded(withPath: compressedScansFolderPath)
        buildFolderIfNeeded(withPath: deCompressedScansFolderPath)
        buildFolderIfNeeded(withPath: middleLineFolderPath)
        buildFolderIfNeeded(withPath: postAlgorithmFolderPath)
        buildFolderIfNeeded(withPath: tempFolderPath)
    }
    
    fileprivate func buildFolderIfNeeded(withPath path : String){
        if !FileManager.default.fileExists(atPath: path) {
            do {
                try FileManager.default.createDirectory(atPath: path, withIntermediateDirectories: true, attributes: nil)
            } catch {
            }
        }
    }
    
    fileprivate func getError(fromOcrResult result : OCRResult) -> SDKScanError{
        return (result.isJaggy || result.isTextOutBad) ? .scanningIsNotStraight
        : (result.isSlow) ? .scanningToSlow
        : (result.isTokenExpired) ? .invalidToken
        : .unknown
    }
    
    fileprivate func createOCREngineIfNeeded(){
        guard let _ = CMocrEngine.getShared() else {
            guard let licenseDataFilePath = Bundle.init(for: ScanMarkerSDK.self).path(forResource: "license", ofType: "txt") else {return}
            guard let licenseDataFile = FileHandle(forReadingAtPath: licenseDataFilePath) else {return}
            let licenseData = licenseDataFile.readDataToEndOfFile()
            let dataPath = Bundle.init(for: ScanMarkerSDK.self).bundlePath
            guard let dataSource = CMocrDirectoryDataSource(directoryPath: dataPath) else {return}
//            let dataSources = Array([dataSource])
            guard let license = CMocrLicense(license: licenseData, applicationId: "iOS_ID") else {return}
            if let engine = CMocrEngine.createSharedEngine(withDataSources: [dataSource], license: license) {
                print(engine)
            }
            return
        }
    }
    
}

//ScanObjecT Delegate functions
extension OCRManager : ScanObjectDelegate {
    
    internal func scanObject(_ scanObject: ScanObject, ocrDidFinishedWithResult result: OCRResult) {
        if scanObject.status == .valid {
            if let image = scanObject.image{
                self.listener?.scanningSuccessWith(image: image, scanner: scanObject.scanner)
            }else if let text = scanObject.text {
                self.listener?.scanningSuccessWith(text: text, scanner: scanObject.scanner)
            }
        }else{
            self.listener?.scanningFailedWith(error: getError(fromOcrResult: result), scanner: scanObject.scanner)
        }
        self.readyScanObjects.removeAll { (so) -> Bool in
            return so.timestamp == scanObject.timestamp
        }
    }
}
