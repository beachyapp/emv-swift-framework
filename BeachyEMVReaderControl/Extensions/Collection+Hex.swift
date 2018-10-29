//
//  Collection.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 18/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation

extension Collection {
    var pairs: [SubSequence] {
        var start = startIndex
        return (0...count/2).map { _ in
            let end = index(start, offsetBy: 2, limitedBy: endIndex) ?? endIndex
            defer { start = end }
            return self[start..<Swift.min(end, endIndex)]
        }
    }
}
