//
//  CBUUIDConstants.swift
//  ScanMarkerSDK
//
//  Created by dave bitton on 11/04/2019.
//  Copyright © 2019 Moveo. All rights reserved.
//

import CoreBluetooth

//UART service - this service allows another device such as a smartphone to exchange any data it wants to with the micro:bit
let urtServiceCBUUID = CBUUID(string: "7C6B5200-A002-B001-C001-0709147C6B52")
//Transmitter character uuid
let txCharacteristicCBUUID = CBUUID(string: "7C6B5200-A002-B001-C002-0709147C6B52")
//Reciever character uuid
let rxCharacteristicCBUUID = CBUUID(string: "7C6B5200-A002-B001-C003-0709147C6B52")
//Device Information service uuid
let deviceInformationServiceCBUUID = CBUUID(string:  "180A")
//Hardware revision character uuid
let hardwareRevisionCBUUID = CBUUID(string: "2A27")


let START_OF_BT_FILE = "ffffff00"
let END_OF_BT_FILE = "ffffff07"
let START_OF_BATTERY_DATA = "ffffff02"
let END_OF_BATTERY_DATA = "ffffff05"

let START_OF_BT_FILE_2 = "ffffff0400"
let END_OF_BT_FILE_2 = "ffffff0407"
let START_OF_BATTERY_DATA_2 = "ffffff0402"
let END_OF_BATTERY_DATA_2 = "ffffff0405"
