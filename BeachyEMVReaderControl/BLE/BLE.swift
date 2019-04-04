//
//  BLE.swift
//  EMV_reader
//
//  Created by Piotr Ilski on 26/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//
import CoreBluetooth

class BLE: NSObject {
    private var devices: Set<BLEDevice> = []
    private var centralManager: CBCentralManager!
    
    var onBLEStateUpdate: ((_ data: String) -> Void)?
    var onBLEAvailableDevicesListUpdate: ((_ devices: Set<BLEDevice>) -> Void)?
    
    func stopScan() {
        self.centralManager.stopScan()
    }
    
    func startScan() {
        self.centralManager.scanForPeripherals(
            withServices: nil,
            options: nil)
    }
    
    override init() {
        super.init()
        
        centralManager = CBCentralManager(delegate: self,
                                          queue: DispatchQueue.main)
    }
}

extension BLE: CBCentralManagerDelegate, CBPeripheralDelegate {
    func centralManagerDidUpdateState(_ central: CBCentralManager) {
        switch central.state {
        case .poweredOn:
            self.centralManager.scanForPeripherals(
                withServices: nil,
                options: nil)
            onBLEStateUpdate?("Bluetooth powered on")
        case .poweredOff:
            self.centralManager.stopScan();
            onBLEStateUpdate?("Bluetooth powered off")
        default:
            onBLEStateUpdate?("Bluetooth sensor in undefined state")
        }
    }
    
    func centralManager(_ central: CBCentralManager,
                        didDiscover peripheral: CBPeripheral,
                        advertisementData: [String : Any],
                        rssi RSSI: NSNumber) {
        let prevCount = devices.count;
        let device = BLEDevice(
            name: peripheral.name ?? "unknown",
            identifier: peripheral.identifier)
        
        devices.insert(device)
        
        if (devices.count != prevCount) {
            onBLEAvailableDevicesListUpdate?(devices)
        }
    }
}
