//
//  BLEDevice.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 10.10.2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import UIKit

class BLEDevice {
    let name: String
    let identifier: UUID
    let isSupportedEmv: Bool
    
    init(name: String, identifier: UUID) {
        self.name = name
        self.identifier = identifier
        
        self.isSupportedEmv =
            name.lowercased().range(of: "idtech") != nil ||
            name.lowercased().range(of: "enzytek") != nil
    }
    
    func getIdentifier() -> UUID {
        return identifier
    }
    
    func getName() -> String {
        return name
    }
}

extension BLEDevice: Equatable {
    static func == (lhs: BLEDevice, rhs: BLEDevice) -> Bool {
        return lhs.name == rhs.name && lhs.identifier == rhs.identifier
    }
}

extension BLEDevice: Hashable {
    var hashValue: Int {
        return name.hashValue ^ identifier.hashValue
    }
}
