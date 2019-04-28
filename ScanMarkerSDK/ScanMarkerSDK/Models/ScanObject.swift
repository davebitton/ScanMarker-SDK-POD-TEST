//
//  ScanObject.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 14/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation
import ScanMarkerSDKPrivate

protocol ScanObjectDelegate :class {
    func scanObject(_ scanObject : ScanObject, ocrDidFinishedWithResult result : OCRResult)
}

class ScanObject{
    
    enum Status {
        case valid, inValid, compressed
    }
    
    fileprivate let minScanLength = 150
    fileprivate let decoder = Decoder()
    fileprivate var engine : CMocrEngine?
    fileprivate(set) var data : Data!
    fileprivate(set) var status : Status!
    fileprivate(set) var text : String?
    fileprivate(set) var image : UIImage?
    fileprivate(set) var timestamp : Int!
    fileprivate(set) var ocrWithImageProcess : Bool = false
    var compressedFilePath : String {
        return String(format: "%@/%lu.bin", compressedScansFolderPath, self.timestamp)
    }
    var tempCompressedFilePath : String {
        return String(format: "%@/%lu_temp.bin", compressedScansFolderPath, self.timestamp)
    }
    var deCompressedFilePath : String {
        return String(format: "%@/%lu.bin", deCompressedScansFolderPath, self.timestamp)
    }
    var picFilePath : String {
        return String(format:"%@/temp.bmp", tempFolderPath)
    }
    var postAlgoFilePath : String {
        return String(format: "%@/%lu.bmp", postAlgorithmFolderPath, self.timestamp)
    }
    weak var delegate : ScanObjectDelegate?
    weak var scanner : SMScanner!
    
    init(data : Data, fromScanner scanner : SMScanner){
        self.data = data.subdata(in: 0...data.count)
        self.scanner = scanner
        if self.data.count < minScanLength {
            self.status = .inValid
        }
        
        self.status = .compressed;
        self.timestamp = Int(Date().timeIntervalSince1970)
        
        //saving data as a file to documents directory
        saveDataToFile()
    }
    
    deinit {
        removeSources()
    }
    
    func doOCR(delegate: ScanObjectDelegate?, withImageProcess : Bool){
        self.ocrWithImageProcess = withImageProcess
        self.delegate = delegate
        if ScanMarkerSDK.sharedInstance.configuration.sdkConfiguration.ocrMode == .online {
            doOnlineOCR()
        }else{
            doOfflineOCR()
        }
    }
    
    fileprivate func doOnlineOCR(){
        NetworkManager().getTokenFromServer(currentScanner: self.scanner) { (optionalToken) in
            guard let token = optionalToken else {
                self.status = .inValid
                self.delegate?.scanObject(self, ocrDidFinishedWithResult: OCRResult(isTokenExpired : true))
                return
            }
            let data64String = NSData(contentsOfFile: self.compressedFilePath)?.base64EncodedString()
            if let data = data64String {
                NetworkManager().OCR(withData64String: data,
                                     token: token,
                                     configuration: ScanMarkerSDK.sharedInstance.configuration,
                                     delegate: self)
            }
        }
    }
    
    fileprivate func doOfflineOCR(){
        self.decode()
        guard let result = self.middleLine() else {return}
        var layout : CMocrLayout?
        var rotationType : TMocrRotationType = MRT_UpsideDown
        let image = UIImage(contentsOfFile: postAlgoFilePath)
        guard let config = getOcrConfiguration() else {return}
        guard let recognitionManager = engine?.newRecognitionManager(with: config) else {return}
        let layoutPTR = AutoreleasingUnsafeMutablePointer<CMocrLayout?>(&layout)
        if recognitionManager.recognizeText(on: image, with: self as? NSObjectProtocol & IMocrRecognitionCallback, store: layoutPTR, rotation: &rotationType) {
            var resultString = ""
            layout?.copyStrings()?.forEach({ (anyObject) in
                if let string = anyObject as? String {
                        resultString.append(string)
                }
            })
            resultString.replacingOccurrences(of: "_ABBYY_", with: "")
            self.text = resultString;
            self.delegate?.scanObject(self, ocrDidFinishedWithResult: OCRResult(text: resultString,isTextOutBad : result.isTextOutBad, isJaggy : result.isJaggy, isSlow : result.isSlow))
        }else{
            var errorCode : TMocrErrorCode = MEC_NoError
            var errorMessage : NSString? = ""
            CMocrEngine.getLastError(&errorCode, message: &errorMessage)
            print(errorMessage)
            print(errorCode)
        }
    }
    
    fileprivate func saveDataToFile(){
        if(!FileManager.default.fileExists(atPath:compressedFilePath)){
            if FileManager.default.createFile(atPath: compressedFilePath, contents: nil, attributes: nil) {
                let url = URL(fileURLWithPath: self.compressedFilePath, isDirectory: false)
                guard self.data.writeTo(url: url, atomically: false) else {
                    self.status = .inValid
                    return
                }
            }
        }
    }
    
    fileprivate func removeSources(){
        do {
            try FileManager.default.removeItem(at: URL(fileURLWithPath: compressedFilePath))
            try FileManager.default.removeItem(at: URL(fileURLWithPath: deCompressedFilePath))
            try FileManager.default.removeItem(at: URL(fileURLWithPath: tempCompressedFilePath))
        }catch let error {
            
        }
    }

}

//OCR Delegate functions
extension ScanObject : OCRDelegate {
    
    internal func OCR(didFinishedWith result: OCRResult) {
        guard !result.isTokenExpired else {
            self.doOCR(delegate: self.delegate, withImageProcess: self.ocrWithImageProcess)
            return
        }
        if result.isTextOutBad || result.isJaggy || result.isSlow || result.text == nil {
            self.status = .inValid
        }
        
        if let text = result.text {
            self.text = text
            self.status = .valid
        }
        if self.ocrWithImageProcess {
            self.decode()
            self.middleLine()
//            HebrewDllLibrary.writeBMP(toPath: picFilePath,
//                                      data: decoder.getRawData(),
//                                      witdh: decoder.getDataWidth(),
//                                      height: decoder.getDataHeight())
            self.image = UIImage(contentsOfFile: postAlgoFilePath)// UIImage(contentsOfFile: picFilePath)?.splitToThird()
            self.delegate?.scanObject(self, ocrDidFinishedWithResult: OCRResult(text: self.text, image: self.image))

        }else{
            self.delegate?.scanObject(self, ocrDidFinishedWithResult: result)
        }
    }
    
}

//private process image functions
extension ScanObject {
    
     fileprivate func decode(){
        let configuration = ScanMarkerSDK.sharedInstance.configuration
        let isRightHanded = configuration.sdkConfiguration.direction == .right
        guard let compressedFilePathPointer = UnsafeMutablePointer<Int8>(mutating: (compressedFilePath as NSString).utf8String) else {return}
        guard let tempCompressedFilePathPointer = UnsafeMutablePointer<Int8>(mutating: (tempCompressedFilePath as NSString).utf8String) else {return}
        if configuration.compressedVersion == 1 {
            //scanner version == 2
            decoder.newDecode(compressedFilePathPointer,
                              destination: tempCompressedFilePathPointer,
                              rightHand: isRightHanded)
        }else{
            //scanner version == 1
            decoder.decode(compressedFilePathPointer, rightHand: isRightHanded)
        }
    }
    
    
    fileprivate func middleLine() -> OCRResult?{
        let resultArraySize  = Int(self.decoder.getDataWidth() * self.decoder.getDataHeight() * 3);
        let resultArray = malloc(resultArraySize);
        memset(resultArray, 0, resultArraySize);
        guard  let resultArrayPTR = resultArray?.assumingMemoryBound(to: UInt8.self) else {return nil}
        let tx = TextImageAnalyzing()
        guard let scanQualityPTR = tx.checkScan(self.decoder.getRawData(),
                                       resultImage: resultArrayPTR,
                                       width: self.decoder.getDataWidth(),
                                       height:  self.decoder.getDataHeight()) else {return nil}
        let qualityDetails = String(cString: scanQualityPTR)
        let result = OCRResult(
            isTextOutBad: qualityDetails.range(of: "textout_bad") != nil,
            isJaggy: qualityDetails.range(of: "jerky_bad") != nil,
            isSlow: qualityDetails.range(of: "slow_bad") != nil
        )
        let mult = DECODER_DEFAULT_WIDTH * DECODER_DEFAULT_HEIGHT;
        let size = mult * 3;
        let width = self.decoder.getDataWidth()
        let height = self.decoder.getDataHeight()
        let outputOCR = malloc(size);
        memset(outputOCR, 0, size);
        
        let outputIC = malloc(mult);
        memset(outputOCR, 0, mult);
        
        var ocr_width = Int32(DECODER_DEFAULT_WIDTH)
        var ocr_height = Int32(DECODER_DEFAULT_HEIGHT)
        let ic_width = Int32(DECODER_DEFAULT_WIDTH)
        let ic_height = Int32(DECODER_DEFAULT_HEIGHT)
        let input = malloc(Int(width * height));
        for i in 0..<width {
            for j in 0..<height {
                input?.storeBytes(of: resultArrayPTR[Int(j * width * 3 + i * 3)], toByteOffset: Int(j * width + i), as: UInt8.self)
            }
        }
        guard  let inputPTR = input?.assumingMemoryBound(to: UInt8.self) else {return nil}
        guard  let outputOCRPTR = outputOCR?.assumingMemoryBound(to: UInt8.self) else {return nil}
        guard  let outputICPTR = outputIC?.assumingMemoryBound(to: UInt8.self) else {return nil}
        guard let postAlgoFilePathPointer = UnsafeMutablePointer<Int8>(mutating: (postAlgoFilePath as NSString).utf8String) else {return nil}
        HebrewDllLibrary().process(withInput: inputPTR, w_In: width, h_In: height, inputRaw: self.decoder.getRawData(), w_Raw: width, h_Raw: height, output_OCR: outputOCRPTR, w_OCR: &ocr_width, h_OCR: &ocr_height, output_IC: outputICPTR, w_IC: ic_width, h_IC: ic_height, italicStep: -1, debugMode: 0, debugFileName: postAlgoFilePathPointer, langusageCode: Int32(ScanMarkerSDK.sharedInstance.configuration.sdkConfiguration.ocrLanguage.codeValue))
        return result
    }
    
    fileprivate func getOcrConfiguration() -> CMocrRecognitionConfiguration? {
        let configuration = ScanMarkerSDK.sharedInstance.configuration
        var langSet = Set<String>()
        langSet.insert("English")
        var ocrConfiguration = CMocrRecognitionConfiguration()
        while ocrConfiguration == nil {
            ocrConfiguration = CMocrRecognitionConfiguration()
        }
        ocrConfiguration?.imageResolution = 300
        ocrConfiguration?.recognitionMode = MRM_Full
        ocrConfiguration?.recognitionConfidenceLevel = MRCL_Level3
        ocrConfiguration?.barcodeTypes = (MBT_ANY1D.rawValue) | (MBT_SQUARE2D.rawValue)
        ocrConfiguration?.unknownLetter = UInt16("^") ?? 1
        ocrConfiguration?.setRecognitionLanguages(langSet)
        
        if configuration.isVerticalScanning{
            ocrConfiguration?.imageProcessingOptions = MIPO_HasCjk.rawValue
            ocrConfiguration?.defaultCodePage = MSCP_JapanSjis
        }else{
            ocrConfiguration?.defaultCodePage = MSCP_Utf8
            ocrConfiguration?.imageProcessingOptions = 0
        }
        return ocrConfiguration
    }
}

extension ScanObject : IMocrRecognitionCallback {

    func called(withProgress progress: Int32, warning: TMocrWarningCode) -> Bool {
        return true
    }

    func onRotationTypeDetected(_ rotationType: TMocrRotationType) {

    }

    func onPrebuiltWordsInfoReady(_ layoutInfo: CMocrPrebuiltLayoutInfo!) {

    }


}
