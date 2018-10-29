//
//  UInt8Array+Hex.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 19/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation

typealias BinaryEntity = (UInt8)

extension Array where Iterator.Element == BinaryEntity {
    func toHexString() -> String {
        return self
            .reduce("", {
                var v = String($1, radix: 16, uppercase: true)
                if (v.count == 1) {
                    v = "0" + v;
                }
                
                return $0 + v
            })
    }
    
    init(hexString: String) {
        self = hexString
            .replacingOccurrences(of: " ", with: "")
            .pairs
            .filter({$0 != ""})
            .map({ UInt8($0, radix: 16)! })
    }
}
