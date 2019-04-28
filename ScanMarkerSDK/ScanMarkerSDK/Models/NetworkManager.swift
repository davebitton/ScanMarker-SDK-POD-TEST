//
//  NetworkManager.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 16/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation
import Alamofire
import SWXMLHash
import StringExtensionHTML
import AEXML

struct OCRResult {
    let text : String?
    let image : UIImage?
    let isTextOutBad : Bool
    let isJaggy : Bool
    let isSlow : Bool
    let isTokenExpired : Bool
    
    init(text : String? = nil, image : UIImage? = nil,isTextOutBad : Bool = false, isJaggy : Bool = false, isSlow : Bool = false, isTokenExpired : Bool = false) {
        self.text = text
        self.image = image
        self.isSlow = isSlow
        self.isJaggy = isJaggy
        self.isTextOutBad = isTextOutBad
        self.isTokenExpired = isTokenExpired
    }
}

protocol OCRDelegate : class{
    func OCR(didFinishedWith result : OCRResult)
}

class NetworkManager {
    
    fileprivate let tokenFileName = "get_token.xml"
    fileprivate let ocrFileName = "run_ocr_v3.xml"
    weak var ocrDelegate : OCRDelegate?
    
    func getTokenFromServer(currentScanner : SMScanner, completion: @escaping ( _ token: String?) -> Void){
        guard let filePath = Bundle.init(for: ScanMarkerSDK.self).path(forResource: self.tokenFileName, ofType: nil) else {return}
        var optionalXmlFile: String? = nil
        do {
            optionalXmlFile = try String(contentsOfFile: filePath, encoding: .utf8)
            let optionalDeviceID = UIDevice.current.identifierForVendor?.uuidString
            if let xmlFile = optionalXmlFile,
                let deviceID = optionalDeviceID {
                let bluetoothID = currentScanner.peripheral.identifier.uuidString
                let soapMessage = String(format: xmlFile, deviceID, bluetoothID)
                self.soapRequest(withMessage: soapMessage) { (response, error) in
                    guard error == nil else {completion(nil); return}
                    guard let res = response else {completion(nil) ;return}
                    if let status = self.getSoapResponseValue(response: res, atPath: ["GetTokenResponse","GetTokenResult", "GetToken"], fromKey: "STATUS") ,
                        let result = self.getSoapResponseValue(response: res, atPath: ["GetTokenResponse","GetTokenResult", "GetToken"], fromKey: "RESULT"),
                        status == "OK",
                        result.count > 0{
                        completion(result)
                    }else{
                        completion(nil)
                    }
                }
            }
        } catch {
            
        }
    }
    
    func OCR(withData64String data64 : String, token : String, configuration : ScanMarkerSDK.OCRConfiguration, delegate : OCRDelegate?){
        self.ocrDelegate = delegate
        guard let filePath = Bundle.init(for: ScanMarkerSDK.self).path(forResource: self.ocrFileName, ofType: nil) else {return}
        var optionalXmlFile: String? = nil
        do {
            optionalXmlFile = try String(contentsOfFile: filePath, encoding: .utf8)
            if let xmlFile = optionalXmlFile{
                let soapMessage = String(format: xmlFile, //ocr file content
                    data64, //data to decompressed as string
                    configuration.sdkConfiguration.ocrLanguage.codeValue,
                    token,
                    (configuration.sdkConfiguration.direction == .left).description,
                    (configuration.isVerticalScanning).description,
                    configuration.compressedVersion,
                    0) //ocrEngine
                self.soapRequest(withMessage: soapMessage) { (response, error) in
                    guard let res = response else {return}
                    guard let status = self.getSoapResponseValue(response: res, atPath: ["RunOCR_V3Response","RunOCR_V3Result", "runOCR"], fromKey: "STATUS") else {return}
                    switch status {
                    case "OK":
                        if let result = self.getSoapResponseValue(response: res, atPath: ["RunOCR_V3Response","RunOCR_V3Result", "runOCR"], fromKey: "RESULT"){
                            self.ocrDelegate?.OCR(didFinishedWith: OCRResult(text: result))
                        }
                    case "ERROR_TOKEN_EXPIRED", "ERROR_TOKEN_NOT_VALID", "ERROR_TOKEN_ABUSE":
                        self.ocrDelegate?.OCR(didFinishedWith: OCRResult(isTokenExpired: true))
                    case "ERROR":
                        if let errorDetails = self.getSoapResponseValue(response: res, atPath: ["RunOCR_V3Response","RunOCR_V3Result", "runOCR"], fromKey: "ERROR_DETAILS"){
                            self.parseOCRerrorDetails(errorDetails)
                        }
                    default:
                        //"ERROR_SERVICE_UNAVAILABLE"
                        //others statuses
                        self.ocrDelegate?.OCR(didFinishedWith: OCRResult(text: nil))
                    }
                }
            }
        }catch {
            
        }
    }
    
    fileprivate func soapRequest(withMessage soapMessage : String , completion: @escaping ( _ responseString: String?, _ error: String?) -> Void){
        let url = URL(string: "http://cloud.a.scanmarker.com/OCRWebServiceMain.asmx?WSDL")
        var req: URLRequest? = nil
        if let url = url {
            req = URLRequest(url: url)
        }
        req?.httpMethod = "POST"
        req?.httpBody = soapMessage.data(using: .utf8)
        req?.addValue("text/xml; charset=utf-8", forHTTPHeaderField: "Content-Type")
        req?.addValue(NSNumber(value: soapMessage.count).stringValue, forHTTPHeaderField: "Content-Length")
        guard let _ = req else {return}
        Alamofire.request(req!)
            .responseString { response in
                if let xmlString = response.result.value {
                    completion(xmlString,nil)
                }else{
                    completion(nil, "error fetching XML")
                }
        }
    }
    
    fileprivate func getSoapResponseValue(response: String , atPath : [String], fromKey key : String) -> String?{
        let xml = SWXMLHash.parse(response)
        let body =  xml["soap:Envelope"]["soap:Body"]
        if let tokenResultElement =  body[atPath[0]][atPath[1]].element {
            let xmlInner = SWXMLHash.parse(tokenResultElement.text.stringByDecodingHTMLEntities)
            return xmlInner[atPath[2]][key].element?.text
        }
        return nil
    }
    
    fileprivate func parseOCRerrorDetails(_ errorDetails : String){
        self.ocrDelegate?.OCR(didFinishedWith: OCRResult(
            isTextOutBad: errorDetails.range(of: "textout_bad") != nil,
            isJaggy: errorDetails.range(of: "jerky_bad") != nil,
            isSlow: errorDetails.range(of: "slow_bad") != nil
        ))
    }
    
}
