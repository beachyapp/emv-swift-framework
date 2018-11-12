//
//  IDT_VP3300.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 28/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation
import IDTPrivate

enum EmvError: Error {
    case deviceIsNotConnected
    case cannotStartTransaction(message: String)
    case cannotParseCardData(message: String)
}

class EmvDevice: NSObject {
    
    var onEmvConnected: (() -> Void)?
    var onEmvDisconnected: (() -> Void)?
    var onEmvTimeout: (() -> Void)?
    var onEmvSendMessage: ((_ message: String) -> Void)?
    var onEmvDataParseError: ((_ errorMessage: String) -> Void)?
    var onEmvDataReceived: ((_ data: String) -> Void)?
    
    override init() {
        super.init()
        
        IDT_VP3300
            .sharedController()
            .delegate = self
    }
    
    /// Enable Transaction Request
    /// Enables CLTS and MSR, waiting for swipe or tap to occur.
    /// Returns IDTEMVData to deviceDelegate::emvTransactionData:()
    ///
    /// - Parameters:
    ///   - amount: amount
    ///   - timeout: timeout
    /// - Throws: cannot start transaction error or device not connected
    func readCC(_ amount: Double, timeout: Int32 = 60) throws -> Void {
        /**
         * Enable Transaction Request
         * Enables CLTS and MSR, waiting for swipe or tap to occur.
         * Returns IDTEMVData to deviceDelegate::emvTransactionData:()
         */
        IDT_VP3300.sharedController().ctls_startTransaction()
        
        if (IDT_VP3300
            .sharedController()
            .device_isConnected(IDT_DEVICE_VP3300_IOS)) {
            
            /**
             * Make sure we cancel any outgoing transaction
             */
            IDT_VP3300.sharedController().msr_cancelMSRSwipe();
            IDT_VP3300.sharedController().device_cancelTransaction();
            
            let rt = IDT_VP3300
                .sharedController()
                .device_startTransaction(amount,
                                         amtOther: 0,
                                         type: 0,
                                         timeout: timeout,
                                         tags: nil,
                                         forceOnline: false,
                                         fallback: true)
            if RETURN_CODE_DO_SUCCESS != rt {
                throw EmvError.cannotStartTransaction(message: String(rt.rawValue, radix: 16))
            }
        } else {
            throw EmvError.deviceIsNotConnected
        }
    }
    
    func connect(friendlyName: String) -> Bool {
        IDT_VP3300
            .sharedController()
            .device_disableBLEDeviceSearch()
        
        IDT_VP3300
            .sharedController()
            .device_setBLEFriendlyName(friendlyName)
        
        return IDT_VP3300
            .sharedController()
            .device_enableBLEDeviceSearch(nil)
    }
    
    func connect(uuid: UUID) -> Bool {
        IDT_VP3300
            .sharedController()
            .device_disableBLEDeviceSearch()
    
        return IDT_VP3300
            .sharedController()
            .device_enableBLEDeviceSearch(uuid)
    }
}

extension EmvDevice: IDT_VP3300_Delegate {
    private func parse(
        encryptedData: String,
        key: String) throws -> String {
        
        let bytesDate: [UInt8] = [UInt8](hexString: encryptedData)
        let ksn = key.replacingOccurrences(of: " ", with: "")
        
        let iv = [UInt8](hexString: "00000000000000000000000000000000")
        let bdk = "0123456789ABCDEFFEDCBA9876543210"
        let sessionKey = try DecryptionUtility.getKey(bdkHex: bdk,
                                                      ksnHex: ksn)
        let keyData = [UInt8](hexString: sessionKey)
        let decrypted = try DecryptionUtility.aesDecrypt(data: bytesDate,
                                                         keyData: keyData,
                                                         iv: iv)!
        
        return decrypted.toHexString().hexToAscii()
    }
    
    func deviceConnected() {
        onEmvConnected?()
    }
    
    func deviceDisconnected() {
        onEmvDisconnected?()
    }
    
    func lcdDisplay(_ mode: Int32, lines: [Any]!) {
        onEmvSendMessage?(lines.description)
    }
    
    func deviceMessage(_ message: String!) {
        onEmvSendMessage?(message)
    }
    
    func emvTransactionData(_ emvData: IDTEMVData!,
                            errorCode error: Int32) {
        if emvData == nil {
            onEmvDataParseError?("Emv data empty")
            
            return
        }
        
        if emvData.resultCodeV2 == EMV_RESULT_CODE_V2_TIME_OUT {
            onEmvTimeout?()
            
            return
        }
        
        // Swipe
        if emvData.cardData != nil {
            do {
                let data = emvData.cardData!.encTrack2
                    .hexEncodedString()
                    .replacingOccurrences(of: " ", with: "")
                let key = emvData.cardData!.ksn
                    .hexEncodedString()
                    .replacingOccurrences(of: " ", with: "")
                
                let decryptedData = try parse(
                    encryptedData: data,
                    key: key)
                
                onEmvDataReceived?(decryptedData)
            } catch {
                onEmvDataParseError?("Cannot parse card data")
            }
            
            return
        }
        
        if emvData.unencryptedTags != nil {
            // Unencrypted tags + empty card data
            // means contactless
            if emvData.cardData == nil {
                let ksnData = emvData.unencryptedTags["FFEE12"] as? Data
                
                if ksnData != nil {
                    let track2DataCandidate = emvData.unencryptedTags["DFEF4D"] as? Data
                    if track2DataCandidate != nil {
                        let dataHex = track2DataCandidate!
                            .hexEncodedString()
                            .replacingOccurrences(of: " ", with: "")
                        let keyHex = ksnData!
                            .hexEncodedString()
                            .replacingOccurrences(of: " ", with: "")
                        do {
                            let decryptedData = try parse(
                                encryptedData: dataHex,
                                key: keyHex)
                            onEmvDataReceived?(decryptedData)
                        } catch {
                            onEmvDataParseError?("Cannot parse card data")
                        }
                    } else {
                        onEmvDataParseError?("Missing Tracka Data")
                    }
                } else {
                    onEmvDataParseError?("Missing KSN")
                }
                
                return
            }
        }
    }
}
