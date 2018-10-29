//
//  String+Hex.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 21/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation
extension String {
    func hexToAscii() -> String {
        let chars = self.pairs.filter({$0 != ""})
            .map({ Character(UnicodeScalar(UInt8($0, radix: 16)!)) })
        return String(chars)
    }
}
