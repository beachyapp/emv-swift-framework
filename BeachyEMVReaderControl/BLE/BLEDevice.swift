//
//  BLEDevice.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 10.10.2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import UIKit

@objc open class BLEDevice: NSObject {
    let name: String
    let identifier: UUID
    @objc public let isSupportedEmv: Bool
    
    init(name: String, identifier: UUID) {
        self.name = name
        self.identifier = identifier
        
        self.isSupportedEmv =
            name.lowercased().range(of: "idtech") != nil ||
            name.lowercased().range(of: "enzytek") != nil
    }
    
    @objc open func getIdentifier() -> UUID {
        return identifier
    }
    
    @objc open func getName() -> String {
        return name
    }
    
    open override var hash: Int {
        return name.hashValue ^ identifier.hashValue
    }
    
    open override func isEqual(_ object: Any?) -> Bool {
        if let dev = object as? BLEDevice {
            return self.name == dev.name && self.identifier == dev.identifier
        } else {
            return false
        }
    }
    
    static func == (lhs: BLEDevice, rhs: BLEDevice) -> Bool {
        return lhs.name == rhs.name && lhs.identifier == rhs.identifier
    }
}
