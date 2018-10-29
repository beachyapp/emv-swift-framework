//
//  DecryptionUtility.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 18/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation

enum EncryptionMode {
    case ENCRYPT
    case DECRYPT
}

enum EncyptionAlgorithm {
    case AES
    case TDES
    case DES
}

class DecryptionUtility {
    
    static func expand3DESKey(hex: String) -> String {
        if (hex.count == 48) {
            return hex
        }
        
        let expandBy = 48 - hex.count
        return hex + hex.prefix(expandBy)
    }
    
    static func extendBDK(bdk: String) -> String {
        /**
         * 24bit long key is in fact a hex with lenght of 48
         */
        if (bdk.count == 48) {
            return bdk
        }
        
        if (bdk.count < 48) {
            let offset = 48 - bdk.count
            let copy = bdk.prefix(offset)
            
            return bdk + copy
        }
        
        return bdk
    }
    
    static func binaryXOR(_ firstHex: String, _ secondHex: String) -> [UInt8] {
        var data1 = [UInt8](hexString: firstHex)
        var data2 = [UInt8](hexString: secondHex)
        
        if ( data1.count < data2.count) {
            while (data1.count < data2.count) {
                data1.insert(0, at: 0)
            }
        }
        
        if ( data2.count < data1.count) {
            while (data2.count < data1.count) {
                data2.insert(0, at: 0)
            }
        }
        
        var bytes = [UInt8]()
        for item in 0..<data1.count {
            bytes.insert(data1[item] ^ data2[item], at: item)
        }
        
        return bytes
    }
    
    static func binaryXOR(_ d1: [UInt8], _ d2: [UInt8]) -> [UInt8] {
        var data1 = d1
        var data2 = d2
        
        if ( data1.count < data2.count) {
            while (data1.count < data2.count) {
                data1.insert(0, at: 0)
            }
        }
        
        if ( data2.count < data1.count) {
            while (data2.count < data1.count) {
                data2.insert(0, at: 0)
            }
        }
        
        var bytes = [UInt8]()
        for item in 0..<data1.count {
            bytes.insert(data1[item] ^ data2[item], at: item)
        }
        
        return bytes
    }
    
    static func binaryAnd(_ firstHex: String, _ secondHex: String) -> [UInt8] {
        var data1 = [UInt8](hexString: firstHex)
        var data2 = [UInt8](hexString: secondHex)
        
        if ( data1.count < data2.count) {
            while (data1.count < data2.count) {
                data1.insert(0, at: 0)
            }
        }
        
        if ( data2.count < data1.count) {
            while (data2.count < data1.count) {
                data2.insert(0, at: 0)
            }
        }
        
        var bytes = [UInt8]()
        for item in 0..<data1.count {
            bytes.insert(data1[item] & data2[item], at: item)
        }
        
        return bytes
    }
    
    static func binaryAnd(_ d1: [UInt8], _ d2: [UInt8]) -> [UInt8] {
        var data1 = d1
        var data2 = d2
        if ( data1.count < data2.count) {
            while (data1.count < data2.count) {
                data1.insert(0, at: 0)
            }
        }
        
        if ( data2.count < data1.count) {
            while (data2.count < data1.count) {
                data2.insert(0, at: 0)
            }
        }
        
        var bytes = [UInt8]()
        for item in 0..<data1.count {
            bytes.insert(data1[item] & data2[item], at: item)
        }
        
        return bytes
    }
    
    /**
     * Get the counter bits from your original (not masked!) 10-byte KSN
     * by ANDing its bottom three bytes with 0x1FFFFF.
     * (Recall that the bottom 21 bits of a KSN comprise the transaction counter.)
     */
    static func getCounterBits(ksnHex: String) -> [UInt8] {
        let bottomThree = Array([UInt8](hexString: ksnHex).suffix(3))
        
        return binaryAnd(bottomThree.toHexString(), "1FFFFF")
    }
    
    static func getKey(bdkHex: String, ksnHex: String) throws -> String {
        let IPEK = try getIPEK(bdkHex: bdkHex, ksnHex: ksnHex)
        let derivedKey = try deriveKey(ksnHex: ksnHex, ipekHex: IPEK)
        
        let initialVector: [UInt8] = [UInt8](hexString: "0000000000000000")
        let dataMask = "0000000000FF00000000000000FF0000"
        let maskedKey = binaryXOR(dataMask, derivedKey)
        
        let expandedMaskedKey = [UInt8](hexString: DecryptionUtility.expand3DESKey(hex: maskedKey.toHexString()))
        
        let left = Array(try desEncrypt(data: Array(maskedKey.prefix(8)),
                                    keyData: expandedMaskedKey,
                                    iv: initialVector)!.prefix(8))
        
        let right = Array(try desEncrypt(data: Array(maskedKey.suffix(8)),
                                     keyData: expandedMaskedKey,
                                     iv: initialVector)!.prefix(8))
        
        
        return Array((left + right)).toHexString()
    }
    
    static func deriveKey(ksnHex: String, ipekHex: String) throws -> String {
        
        let bottomEightFromKSN = Array([UInt8](hexString: ksnHex).suffix(8))
        let baseKSN = binaryAnd("FFFFFFFFFFE00000", bottomEightFromKSN.toHexString())
        
        let counter = getCounterBits(ksnHex: ksnHex)
        var currKey = ipekHex
        
        let counterInt = Int(counter.toHexString(), radix: 16)!
        var baseKSNInt = Int(baseKSN.toHexString(), radix: 16)!
        
        
        var shiftReg = 0x100000
        var pass = 0
        
        while(shiftReg > 0) {
            if ((shiftReg & counterInt) > 0) {
                baseKSNInt |= shiftReg
                currKey = try generateKey(
                    key: currKey,
                    ksn: String(format:"%llX", baseKSNInt)
                ).toHexString();
                
                let ksnHex = String(format:"%llX", baseKSNInt)
                let keyHex = currKey
                
                print("Pass \(pass), baseKSN:\(ksnHex) key: \(keyHex)")
                
                pass += 1
            }
            
            shiftReg >>= 1
        }
        
        return currKey
    }
    
    static func generateKey(key: String, ksn: String) throws -> [UInt8] {
        let mask = "C0C0C0C000000000C0C0C0C000000000"
        let maskedKey =  binaryXOR(mask, key);
        
        let left  = try encryptRegister(key: maskedKey,
                                        ksn: [UInt8](hexString: ksn));
        let right = try encryptRegister(key: [UInt8](hexString: key),
                                        ksn: [UInt8](hexString: ksn));
        
        return left + right
    }
    
    static func encryptRegister(key: [UInt8], ksn: [UInt8]) throws -> [UInt8] {
        let bottomEight = Array(key.suffix(8))
        let topEight = Array(key.prefix(8))
        let initialVector: [UInt8] = [UInt8](hexString: "0000000000000000")
        let bottomEightXORKSN = binaryXOR(bottomEight, ksn)
        let desEncrypted = Array(try singleDesEncrypt(data: bottomEightXORKSN,
                                                  keyData: topEight,
                                                  iv: initialVector)!.prefix(8))
        
        return binaryXOR(bottomEight, desEncrypted)
    }
    
    static func getIPEK(bdkHex: String, ksnHex: String) throws -> String {
        
        let extendedBdk = DecryptionUtility.extendBDK(bdk: bdkHex)
        let maskedKSN = binaryAnd(ksnHex, "FFFFFFFFFFFFFFE00000")
        
        let bytesDate = maskedKSN
        let keyData: [UInt8] = [UInt8](hexString: extendedBdk)
        let initialVector: [UInt8] = [UInt8](hexString: "0000000000000000")
        let leftIPEK = try desEncrypt(data: bytesDate,
                                  keyData: keyData,
                                  iv: initialVector)!
        //take only 8 bytes
        let leftHalfOfIPEK = Array(leftIPEK[..<8])
        
        let xorKey = binaryXOR(bdkHex, "C0C0C0C000000000C0C0C0C000000000")
        let xorExpandedKey = [UInt8](
            hexString: DecryptionUtility.expand3DESKey(hex: xorKey.toHexString()))
        
        
        let rightIPEK = try desEncrypt(data: maskedKSN,
                                   keyData: xorExpandedKey,
                                   iv: initialVector)!
        let rightHalfOfIPEK = Array(rightIPEK[..<8])
        let IPEK = leftHalfOfIPEK + rightHalfOfIPEK
        
        return IPEK.toHexString()
//        } catch {
//            fatalError(error.localizedDescription)
//        }
    }
    
    static func parseData(data: [UInt8],
                    keyData: [UInt8],
                    iv: [UInt8],
                    mode: EncryptionMode,
                    algorythm: EncyptionAlgorithm,
                    options: CCOptions = UInt32(kCCOptionECBMode + kCCOptionPKCS7Padding)) throws -> [UInt8]? {
        var blockSize: Int = Int(data.count)
        var operation: CCOperation
        var algorithm:  CCAlgorithm
        var keyLength: Int
        
        switch algorythm {
        case .AES:
            blockSize += kCCBlockSizeAES128
            keyLength = size_t(kCCKeySizeAES128)
            algorithm = UInt32(kCCAlgorithmAES128)
        case .DES:
            blockSize += kCCBlockSizeDES
            keyLength = size_t(kCCKeySizeDES)
            algorithm = UInt32(kCCAlgorithmDES)
        case .TDES:
            blockSize += kCCBlockSize3DES
            keyLength = size_t(kCCKeySize3DES)
            algorithm = UInt32(kCCAlgorithm3DES)
        }
        
        switch mode {
        case .DECRYPT:
            operation = UInt32(kCCDecrypt)
        case .ENCRYPT:
            operation = UInt32(kCCEncrypt)
        }
        
        var numBytesEncrypted :size_t = 0
        
        let cryptData = NSMutableData(length: blockSize)!
        let cryptStatus = CCCrypt(operation,
                                  algorithm,
                                  options,
                                  keyData,
                                  keyLength,
                                  iv,
                                  data,
                                  data.count,
                                  cryptData.mutableBytes,
                                  cryptData.length,
                                  &numBytesEncrypted)
        
        
        if UInt32(cryptStatus) == UInt32(kCCSuccess) {
            cryptData.length = Int(numBytesEncrypted)
            
            return [UInt8](cryptData as Data)
            
        } else {
            fatalError("Unable to parse card data: \(cryptStatus)")
        }
        
        return nil
    }
    
    static func singleDesEncrypt(data: [UInt8], keyData: [UInt8], iv: [UInt8]) throws -> [UInt8]? {
        return try parseData(data: data, keyData: keyData, iv: iv, mode: .ENCRYPT, algorythm: .DES)
    }
    
    static func desEncrypt(data: [UInt8], keyData: [UInt8], iv: [UInt8]) throws -> [UInt8]? {
        return try parseData(data: data, keyData: keyData, iv: iv, mode: .ENCRYPT, algorythm: .TDES)
    }
    
    static func aesDecrypt(data: [UInt8], keyData: [UInt8], iv: [UInt8]) throws -> [UInt8]? {
        let options:   CCOptions   = UInt32(kCCOptionECBMode + kCCOptionECBMode)
        
        return try parseData(data: data, keyData: keyData, iv: iv, mode: .DECRYPT, algorythm: .AES, options: options)
    }
}
