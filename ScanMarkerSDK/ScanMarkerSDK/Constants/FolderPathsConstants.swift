//
//  FolderPathsConstants.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 17/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import Foundation

let documentsDirectory = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
let compressedScansFolderPath = URL(fileURLWithPath: documentsDirectory).appendingPathComponent("Compressed Scans").relativePath
let deCompressedScansFolderPath = URL(fileURLWithPath: documentsDirectory).appendingPathComponent("DeCompressed Scans").relativePath
let middleLineFolderPath = URL(fileURLWithPath: documentsDirectory).appendingPathComponent("MiddleLine Scans").relativePath
let postAlgorithmFolderPath = URL(fileURLWithPath: documentsDirectory).appendingPathComponent("PostAlgorithm Scans").relativePath
let tempFolderPath = URL(fileURLWithPath: documentsDirectory).appendingPathComponent("Temp Files").relativePath
