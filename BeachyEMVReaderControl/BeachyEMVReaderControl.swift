//
//  BeachyEMVReaderControl.swift
//  BeachyEMVReaderControl
//
//  Created by Piotr Ilski on 30/10/2018.
//  Copyright © 2018 Beachy. All rights reserved.
//

import Foundation

@objc public protocol BeachyEMVReaderControlProtocol {
    func bluetoothStatusUpdate(status: String)
    func bluetoothAvailableDevicesListUpdate(devices: Set<BLEDevice>)
    
    func readerConnected()
    func readerDisconnected()
    
    func readerDataParseError(errorMessage: String)
    func readerData(data: String)
    func readerSendsMessage(message: String)
}

@objc open class BeachyEMVReaderControl: NSObject {
    
    @objc open var delegate: BeachyEMVReaderControlProtocol?
    @objc public static var shared = BeachyEMVReaderControl()
    
    private var bluetoothControl = BLE()
    private var emvDeviceControl = EmvDevice()
    
    override init() {
        super.init()

        initializeBluetooth()
        initializeEmv()
    }
    
    
    /// Configure EMV sleep and power off times
    /// - Parameters:
    ///   - sleepTimeInSec: sleep time in seconds
    ///   - powerOffTimeInSec: power off time in seconds
    /// - Returns: 0 - ok,
    ///            anything else - error
    @objc open func configureSleepModeAndPowerOffTimes(
        sleepTimeInSec: Int,
        powerOffTimeInSec: Int) -> UInt32 {
        return emvDeviceControl.setReaderSleepAndPowerOffTime(
            sleepTimeInSec: sleepTimeInSec,
            powerOffTimeInSec: powerOffTimeInSec)
    }
    
    /// Send a command to EMV reader to become active and
    /// start waiting for swipe/contactless payment
    /// - Parameters:
    ///   - amount: amount
    ///   - timeout: timeout
    /// - Returns:  0 - ok,
    ///             1 - cannot start transaction,
    ///             2 - device is not connected,
    ///             3 - unknown error
    @objc open func readCardData(_ amount: Double,
                                 timeout: Int32 = 60) -> Int {
        do {
            try emvDeviceControl.readCC(amount, timeout: timeout)
            
            return 0
        } catch EmvError.cannotStartTransaction( _) {
            return 1
        } catch EmvError.deviceIsNotConnected {
            return 2
        } catch {
            return 3
        }
    }
    
    /// Connect to nearest BLE Reader that matches
    /// set friendly name.
    /// - Parameter friendlyName: device friendly name, like IDT_*
    /// - Returns: true if connecting
    @objc open func connect(friendlyName: String) -> Bool {
        return emvDeviceControl.connect(friendlyName: friendlyName)
    }
    
    /// Connect to BLE Reader using UUID.
    ///
    /// - Parameter uuid: device UUID
    /// - Returns: true if connecting
    @objc open func  connect(uuid: UUID) -> Bool {
        return emvDeviceControl.connect(uuid: uuid)
    }
 
    
    /// Initialize low-energy bluetooth handlers
    private func initializeBluetooth() {
        bluetoothControl.onBLEStateUpdate = {
            [weak self] (message: String) in
            self?
                .delegate?
                .bluetoothStatusUpdate(status: message)
        }
        
        bluetoothControl.onBLEAvailableDevicesListUpdate = {
            [weak self] (devices: Set<BLEDevice>) in
            self?
                .delegate?
                .bluetoothAvailableDevicesListUpdate(
                    devices: devices)
        }
    }
    
    /// Initialize EMV handlers
    private func initializeEmv() {
        emvDeviceControl.onEmvConnected = {
            [weak self] () in self?.delegate?.readerConnected()
        }
        
        emvDeviceControl.onEmvDisconnected = {
            [weak self] () in self?.delegate?.readerDisconnected()
        }
        
        emvDeviceControl.onEmvDataParseError = {
            [weak self] (error: String) in
                self?
                    .delegate?
                    .readerDataParseError(errorMessage: error)
        }
        
        emvDeviceControl.onEmvTimeout = {
            [weak self] () in
            self?
                .delegate?
                .readerDataParseError(errorMessage: "Timed out")
        }
        
        emvDeviceControl.onEmvSendMessage = {
            [weak self] (message: String) in
            self?
                .delegate?
                .readerSendsMessage(message: message)
        }
        
        emvDeviceControl.onEmvDataReceived = {
            [weak self] (data: String) in
            self?
                .delegate?
                .readerData(data: data)
        }
    }
}
