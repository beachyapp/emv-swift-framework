//
//  IDT_VP3300.h
//  IDTech
//
//  Created by Randy Palermo on 10/29/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"

/** Protocol methods established for IDT_VP3300 class  **/
@protocol IDT_VP3300_Delegate <NSObject>
@optional
-(void) deviceConnected; //!<Fires when device connects.  If a connection is established before the delegate is established (no delegate to send initial connection notification to), this method will fire upon establishing the delegate.
-(void) deviceDisconnected; //!<Fires when device disconnects.
- (void) plugStatusChange:(BOOL)deviceInserted; //!<Monitors the headphone jack for device insertion/removal.
//!< @param deviceInserted TRUE = device inserted, FALSE = device removed
- (void) dataInOutMonitor:(NSData*)data  incoming:(BOOL)isIncoming; //!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!< @param data The serial data represented as a NSData object
//!< @param isIncoming The direction of the data
//!<- <c>TRUE</c> specifies data being received from the device,
//!<- <c>FALSE</c> indicates data being sent to the device.

- (void) swipeMSRData:(IDTMSRData*)cardData;//!<Receives card data from MSR swipe.
//!< @param cardData Captured card data from MSR swipe

- (void) deviceMessage:(NSString*)message;//!<Receives messages from the framework
//!< @param message String message transmitted by framework

- (void) bypassData:(NSData*)data;//!<When bypass output is enabled, all data intended for the current device will be sent here .
//!< @param data The data intended for the device




/**
 LCD Display Request
 During an EMV transaction, this delegate will receive data to clear virtual LCD display, display messages, display menu, or display language.  Applies to UniPay III
 
 @param mode LCD Display Mode:
 - 0x01: Menu Display.  A selection must be made to resume the transaction
 - 0x02: Normal Display get function key.  A function must be selected to resume the transaction
 - 0x03: Display without input.  Message is displayed without pausing the transaction
 - 0x04: List of languages are presented for selection. A selection must be made to resume the transaction
 - 0x10: Clear Screen. Command to clear the LCD screen
 
 */

- (void) lcdDisplay:(int)mode  lines:(NSArray*)lines;

/**
 PIN Request
 During an EMV transaction, this delegate will receive data that is a request to collect a PIN
 
 @param mode PIN Mode:
 - EMV_PIN_MODE_CANCEL = 0X00,
 - EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
 - EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
 - EMV_PIN_MODE_OFFLINE_PIN = 0X03
 @param key Either DUKPT or SESSION, depending on mode. If offline plaintext, value is nil
 @param PAN PAN for calculating PINBlock
 @param startTO Timeout value to start PIN entry
 @param intervalTO Timeout value between key presses
 @param language "EN"=English, "ES"=Spanish, "ZH"=Chinese, "FR"=French
 
 */

- (void) pinRequest:(EMV_PIN_MODE_Types)mode  key:(NSData*)key  PAN:(NSData*)PAN startTO:(int)startTO intervalTO:(int)intervalTO language:(NSString*)language;



/**
 EMV Transaction Data
 
 This protocol will receive results from IDT_Device::startEMVTransaction:otherAmount:timeout:cashback:additionalTags:()
 
 
 @param emvData EMV Results Data.  Result code, card type, encryption type, masked tags, encrypted tags, unencrypted tags and KSN
 
 @param error The error code as defined in the errors.h file
 
 
 */
- (void) emvTransactionData:(IDTEMVData*)emvData errorCode:(int)error;

@end

/**
 Class to drive the IDT_VP3300 device
 */
@interface IDT_VP3300 : NSObject<IDT_Device_Delegate>{
    id<IDT_VP3300_Delegate> delegate;
    id<IDT_VP3300_Delegate> bypassDelegate;

}

@property(strong) id<IDT_VP3300_Delegate> delegate;  //!<- Reference to IDT_VP3300_Delegate.
@property(strong) id<IDT_VP3300_Delegate> bypassDelegate;  //!<- Reference to IDT_VP3300_Delegate.



/**
 * SDK Version
 *
 Returns the current version of IDTech.framework
 
 @return  Framework version
 */
+(NSString*) SDK_version;

/**
 * Singleton Instance
 *
 Establishes an singleton instance of IDT_VP3300 class.
 
 @return  Instance of IDT_VP3300
 */
+(IDT_VP3300*) sharedController;

/**
 *Close Device
 */

-(void) close;

/**
 * Connect To Audio Reader
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the USB port.
 
 */

-(void) device_connectToUSB;



/**
 Cancels Transaction request (swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) ctls_cancelTransaction;



/**
 Cancels Transaction request (EMV).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) emv_cancelTransaction;



/**
 Cancels Transaction request (EMV, swipe or CTLS).
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) device_cancelTransaction;


/**
 * Get Configuration Group
 *
 Retrieves the Configuration for the specified Group. Group 0 = terminal settings.
 
 @param group Configuration Group
 @param response Group TLV returned as a dictionary
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) ctls_getConfigurationGroup:(int)group response:(NSDictionary**)response;


/**
 Process Bypass Response.
 
 When output is being bypassed, and a command is received in the delegate redirectedOutput and sent to the device, the device rexponse must be sent to this
 method so the SDK can finishing processing the results;
 
 @param data The data received from the device that the SDK must process to complete the executing command
 */
-(void) processBypassResponse:(NSData*)data;

/**
 * Set Bypass Delegate
 *
 Sets the bypass delegate. .
 
 @param del  IDT_Device_Delegate delegate.
 
 
 */

-(void) assignBypassDelegate:(id<IDT_VP3300_Delegate>)del;




/**
 * Remove All Certificate Authority Public Key
 *
 Removes all the CAPK for CTLS
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()

 */
-(RETURN_CODE) ctls_removeAllCAPK;

/**
 * Remove Application Data by AID
 *
 Removes the Application Data for CTLS as specified by the AID name passed as a parameter
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010") Must be between 5 and 16 bytes
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK for CTLS as specified by the RID/Index
 
 @param capk 6 byte CAPK =  5 bytes RID + 1 byte INDEX
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeCAPK:(NSData*)capk;

/**
 * Remove Configuration Group
 *
 Removes the Configuration as specified by the Group.  Must not by group 0
 
 @param group Configuration Group
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_removeConfigurationGroup:(int)group;


/**
 * Retrieve AID list
 *
 Returns all the AID names installed on the terminal for CTLS.
 
 @param response  array of AID name as NSData*
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_retrieveAIDList:(NSArray**)response;

/**
 * Retrieve Application Data by AID
 *
 Retrieves the CTLS Application Data as specified by the AID name passed as a parameter.
 
 @param AID Name of ApplicationID as Hex String (example "A0000000031010"). Must be between 5 and 16 bytes
 @param response  The TLV elements of the requested AID
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)response;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPKfor CTLS as specified by the RID/Index  passed as a parameter.
 
 @param capk 6 bytes CAPK = 5 bytes RID + 1 byte Index
 @param key Response returned as a CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPK:(NSData*)capk key:(NSData**)key;


/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index installed on the terminal for CTLS.
 
 @param keys NSArray of NSData CAPK name  (RID + Index)
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */
-(RETURN_CODE)  ctls_retrieveCAPKList:(NSArray**)keys;

/**
 * Retrieve Terminal Data
 *
 Retrieves the Terminal Data for CTLS. This is configuration group 0 (Tag FFEE - > FFEE0100).  The terminal data
 can also be retrieved by ctls_getConfiguraitonGroup(0).
 
 @param tlv Response returned as a TLV
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE)  ctls_retrieveTerminalData:(NSData**)tlv;

/**
 * Set Application Data by AID
 *
 Sets the Application Data for CTLS as specified by TLV data
 
 @param tlv  Application data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 The second tag of the TLV data must be the AID (9F06)
 
 Example valid TLV, for Group #2, AID a0000000035010:
 "ffe401029f0607a0000000051010ffe10101ffe50110ffe30114ffe20106"
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE)  ctls_setApplicationData:(NSData*)tlv;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK for CTLS as specified by the CAKey structure
 
 @param key CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE)  ctls_setCAPK:(NSData*)key;


/**
 * Set Configuration Group
 *
 Sets the Configuration Group for CTLS as specified by the TLV data
 
 @param tlv  Configuration Group Data in TLV format
 The first tag of the TLV data must be the group number (FFE4).
 A second tag must exist
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setConfigurationGroup:(NSData*)tlv;


/**
 * Set Terminal Data
 *
 Sets the Terminal Data for CTLS as specified by the TLV.  The first TLV must be Configuration Group Number (Tag FFE4).  The terminal global data
 is group 0, so the first TLV would be FFE40100.  Other groups can be defined using this method (1 or greater), and those can be
 retrieved with ctls_getConfigurationGroup(int group), and deleted with ctls_removeConfigurationGroup(int group).  You cannot
 delete group 0.
 
 @param tlv TerminalData configuration data
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) ctls_setTerminalData:(NSData*)tlv;




/**
 * Start a CTLS Transaction Request
 *
 Authorizes the CTLS transaction for an CTLS card
 
 The tags will be returned in the callback routine.
 
 @param amount Transaction amount value  (tag value 9F02)
 @param exponent Number of characters after decimile point
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 *
 * NOTE ON APPLEPAY VAS:
 * To enable ApplePay VAS, first a merchant record must be defined in one of the six available index positions (1-6) using device_setMerchantRecord, then container tag FFEE06
 * must be sent as part of the additional tags parameter of ctls_startTransaction.  Tag FFEE06 must contain tag 9F26 and 9F22, and can optionanally contain tags 9F2B and DFO1.
 * Example FFEE06189F220201009F2604000000009F2B050100000000DF010101
 * 9F22 = two bytes = ApplePay Terminal Applicaiton Version Number.  Hard defined as 0100 for now. (required)
 * 9F26 = four bytes = ApplePay Terminal Capabilities Information (required)
 *  - Byte 1 = RFU
 *  - Byte 2 = Terminal Type
 *  - - Bit 8 = VAS Support  (1=on, 0 = off)
 *  - - Bit 7 = Touch ID Required  (1=on, 0 = off)
 *  - - Bit 6 = RFU
 *  - - Bit 5 = RFU
 *  - - Bit 1,2,3,4
 *  - - - 0 = Payment Terminal
 *  - - - 1 = Transit Terminal
 *  - - - 2 = Access Terminal
 *  - - - 3 = Wireless Handoff Terminal
 *  - - - 4 = App Handoff Terminal
 *  - - - 15 = Other Terminal
 *  - Byte 3 = RFU
 *  - Byte 4 = Terminal Mode
 *  - - 0 = ApplePay VAS OR ApplePay
 *  - - 1 = ApplePay VAS AND ApplePay
 *  - - 2 = ApplePay VAS ONLY
 *  - - 3 = ApplePay ONLY
 *  9F2B = 5 bytes = ApplePay VAS Filter.  Each byte filters for that specific merchant index  (optional)
 *  DF01 = 1 byte = ApplePay VAS Protocol.  (optional)
 *  - - Bit 1 : 1 = URL VAS, 0 = Full VAS
 *  - - Bit 2 : 1 = VAS Beeps, 0 = No VAS Beeps
 *  - - Bit 3 : 1 = Silent Comm Error, 2 = EMEA Comm Error
 *  - - Bit 4-8 : RFU
 *
 */
-(RETURN_CODE) ctls_startTransaction:(double)amount type:(int)type timeout:(int)timeout tags:(NSMutableDictionary *)tags;



/**
 * Enable Transaction Request
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:()
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) ctls_startTransaction;



/**
 * Cancel Connect To Audio Reader
 * @return RETURN_CODE
 *
 Cancels a connection attempt to an IDTech MSR device connected via the audio port.
 
 */


-(RETURN_CODE) device_cancelConnectToAudioReader;
/**
 * Connect To Audio Reader
 * @return RETURN_CODE
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the audio port.
 
 */

-(RETURN_CODE) device_connectToAudioReader;


/**
 * Polls device for Firmware Version
 
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;


/**
 * Begins searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 * @param identifier This will only connect to a device with this calculated UUID identifier.  If nil is passed, it will connect to the first devices with the default friendly name (IDT_VP3300::device_getBLEFriendlyName / IDT_VP3300::device_setBLEFriendlyName)
 *
 * @return bool  If successful, polling has started
 *
 * Any of the following BLE status messages may be returned to the deviceMessage delegate:
 - This device does not support Bluetooth Low Energy.
 - This app is not authorized to use Bluetooth Low Energy.
 - Bluetooth on this device is currently powered off.
 - The BLE Manager is resetting; a state update is pending.
 - Bluetooth LE is turned on and ready for communication.
 - The state of the BLE Manager is unknown.
 *
 * Note: a Devices UUID is calculated by the iOS device using a combiniation of the iOS device UUID and the BLE device MAC address.  This value is not known until after it connects for the first time, and then every time after that, it will be the same value.  This value can be retrieved by IDT_Device::connectedBLEDevice() after the device connects.
 */
-(bool) device_enableBLEDeviceSearch:(NSUUID*)identifier;



/**
 * Get BLE Friendly Name
 *
 *
 * @return NSString  Returns the default friendly name to be used when discovering any BLE devices
 *
 */
-(NSString*) device_getBLEFriendlyName;


/**
 * Set BLE Friendly Name
 *
 *
 * @param friendlyName  Sets the default friendly name to be used when discovering any BLE devices
 *
 */
-(void) device_setBLEFriendlyName:(NSString*)friendlyName;


/**
 * Stops searching for Bluetooth Low Energy devices in range
 - VP3300
 *
 *
 * @return bool  If successful, polling was in progress and has stopped. If unsuccessful, BLE Device Search was not in progress.
 *
 * NOTE:  BLE only scans when there are no devices currently connected. After the SDK connects to any IDTech device, the scanning will pause automatically.
 */
-(bool) device_disableBLEDeviceSearch;

/**
 * Returns the UUID of the connected BLE device
 - VP3300
 *
 * @return NSUUID  UUID of the connected BLE device.  Returns nil if no BLE device connected.
 *
 */
-(NSUUID*) device_connectedBLEDevice;



/**
 * Is Audio Reader Connected
 
 *
 Returns value on device connection status when device is an audio-type connected to headphone plug.
 
 @return BOOL True = Connected, False = Disconnected
 
 */

-(BOOL) device_isAudioReaderConnected;


/**
 * Get Transaction Results
 Gets the transaction results when the reader is functioning in "Auto Poll" mode
 *
 * @param result The transaction results
 *
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString().  When no data is available, return code = RETURN_CODE_NO_DATA_AVAILABLE
 *
 */
-(RETURN_CODE)  device_getAutoPollTransactionResults:(IDTEMVData**)result;

/**
 * Get Response Code String
 *
 Interpret a response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;

/**
 Is Device Connected
 
 Returns the connection status of the requested device
 
 @param device Check connectivity of device type
 
 @code
 typedef enum{
 IDT_DEVICE_VP3300_IOS = 8
 IDT_DEVICE_VP3300_OSX_USB = 9
 }IDT_DEVICE_Types;
 
 @endcode
 */
-(bool) device_isConnected:(IDT_DEVICE_Types)device;

/**
 * Send NEO IDG Command
 Send a NEO IDG ViVOtech 2.0 command
 *
 * @param command  One byte command as per NEO IDG Reference Guide
 * @param subCommand  One byte sub-command as per NEO IDG Reference Guide
 * @param data  Command data (if applicable)
 * @param response  Returns next Command response
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMagIII::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) device_sendIDGCommand:(unsigned char)command subCommand:(unsigned char)subCommand data:(NSData*)data response:(NSData**)response;

/**
 * Set Volume To Audio Reader
 
 *
 Set the iPhone’s volume for command communication with audio-based readers. The the range of iPhone’s volume is from 0.1 to 1.0.
 
 @param val Volume level from 0.1 to 1.0
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setAudioVolume:(float)val;

/**
 * Set Pass Through
 
 Sets Pass-Through mode on VP3300
 *
 @param enablePassThrough  TRUE = Pass through enabled
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_setPassThrough:(BOOL)enablePassThrough;


/**
 * Send Burst Mode
 *
 * Sets the burst mode forthe device.
 *
 * @param mode 0 = OFF, 1 = Always On, 2 = Auto Exit
 
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 */
-(RETURN_CODE)  device_setBurstMode:(int) mode;


/**
 * Send Poll Mode
 *
 * Sets the poll mode forthe device. Auto Poll keeps reader active, Poll On Demand only polls when requested by terminal
 *
 * @param mode 0 = Auto Poll, 1 = Poll On Demand
 
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 */
-(RETURN_CODE) device_setPollMode:(int) mode;


/**
 * Start Remote Key Injection
 
 *
 Attempts to perform a Remote Key Injection with IDTech's RKI servers.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) device_startRKI;

/**
 * Authenticate Transaction
 
 Authenticates a transaction after startTransaction successfully executes.
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function must be called after a result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS returned to emvTransactionData delegate protocol is received after a startTransaction call.  If auto authorize is ENABLED (default), this method will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS after startTransaction.  The auto authorize can be enabled/disabled with IDT_DEVICE::disableAutoAuthenticateTransaction:()
 *
 The purpose of this step is to allow the merchant the chance to evaluate the data captured from the matching Application (if found) before the kernel authenticates the transaction data.  This would allow, for instance, the merchant to be told what card is being used, and if it is a specific type (like a store card), perform an action like reducing the amount before proceeding (as a promotion in using that card).
 
 @param tags Any tags to modify original tags submitted with startTransaction.  Passed as NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_authenticateTransaction:(NSData*)tags;
/**
 * Callback Response LCD Display
 *
 Provides menu selection responses to the kernel after a callback was received lcdDisplay delegate.
 
 @param mode The choices are as follows
 - 0x00 Cancel
 - 0x01 Menu Display
 - 0x02 Normal Display get Function Key  supply either 0x43 ('C') for Cancel, or 0x45 ('E') for Enter/accept
 - 0x08 Language Menu Display
 
 @param selection Line number in hex (0x01, 0x02), or 'C'/'E' of function key
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) emv_callbackResponseLCD:(int)mode selection:(unsigned char) selection;

/**
 * Callback Response PIN Request
 *
 Provides PIN data  to the kernel after a callback was received pinRequest delegate.
 
 @param mode PIN Mode:
 - EMV_PIN_MODE_CANCEL = 0X00,
 - EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
 - EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
 - EMV_PIN_MODE_OFFLINE_PIN = 0X03
 @param KSN  Key Serial Number. If no pairing and PIN is plaintext, value is nil
 @param PIN PIN data, encrypted.  If no pairing, PIN will be sent plaintext
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) emv_callbackResponsePIN:(EMV_PIN_MODE_Types)mode KSN:(NSData*)KSN PIN:(NSData*)PIN;

/**
 * Complete EMV Transaction Online Request
 *
 Completes an online EMV transaction request by the card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 @param isSuccess Determines if connection to host was successful:
 - TRUE: Online processing with the host (issuer) was completed
 - FALSE: Online processing could not be completed due to connection error with the host (issuer). No further data (tags) required.
 @param tags Host response tag (see below)
 
 \par Host response tag:
 
 Tag | Length | Description
 ----- | ----- | -----
 8A | 2 | Data element Authorization Response Code. Mandatory
 91 | 8-16 | Issuer Authentication Data. Optional
 71 | 0-256 | Issuer Scripts. Optional
 72 | 0-256 | Issuer Scripts. Optional
 DFEE1A | 0-256 | Tag list of additional tags to return
 
 Tag DFEE1A will force additional tags to be returned.  Example DFEE1A049F029F03 will return tags
 9F02 and 9F03 with the response
 
 *  @return RETURN_CODE:
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0xFE00: Authorization Accepted - RETURN_CODE_EMV_AUTHORIZATION_ACCEPTED
 - 0xFE01: Online Failure - RETURN_CODE_EMV_AUTHORIZATION_UNABLE_TO_GO_ONLINE
 - 0xFE02: Technical Issue - RETURN_CODE_EMV_AUTHORIZATION_TECHNICAL_ISSUE
 - 0xFE03: Declined - RETURN_CODE_EMV_AUTHORIZATION_DECLINED
 - 0xFE04: Issuer Referral - RETURN_CODE_EMV_AUTHORIZATION_ISSUER_REFERRAL
 
 
 
 */
-(RETURN_CODE) emv_completeOnlineEMVTransaction:(BOOL)isSuccess hostResponseTags:(NSData*)tags;



/**
 * Disable Auto Authenticate Transaction
 *
 If auto authenticate is DISABLED, authenticateTransaction must be called after a successful startEMV execution.
 
 @param disable  FALSE = auto authenticate ENABLED, TRUE = auto authenticate DISABLED
 
 */
-(void) emv_disableAutoAuthenticateTransaction:(BOOL)disable;



/**
 * Polls device for EMV L2 Version
 *
 * @param response Response returned of Level 2 Version
 *
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) emv_getEMVL2Version:(NSString**)response;

/**
 * Remove Application Data by AID
 *
 This will REMOVE the an AID configuration file and all the tlv data associated with that AID.
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeApplicationData:(NSString*)AID;

/**
 * Remove Certificate Authority Public Key
 *
 Removes the CAPK as specified by the RID/Index passed as a parameter in the CAKey structure
 
 @param rid RID of the key to remove
 @param index Index of the key to remove
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCAPK:(NSString*)rid index:(NSString*)index ;

/**
 Sets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 */
-(RETURN_CODE) emv_setTerminalMajorConfiguration:(int)configuration;

/**
 Gets the terminal major configuration in ICS .
 @param configuration A configuration value, range 1-5
 - 1 = 1C
 - 2 = 2C
 - 3 = 3C
 - 4 = 4C
 - 5 = 5C
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 */
-(RETURN_CODE) emv_getTerminalMajorConfiguration:(NSUInteger**)configuration;


/**
 * Remove Certificate Revocation List
 *
 Removes all CRLEntry entries
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeCRLList;


/**
 * Remove Terminal Data
 *
 
 This will remove ALL configurable TLV data associated with the terminals Kernel configuration.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_removeTerminalData;

/**
 * Retrieve AID list
 *
 Returns all the AID names on the terminal.  Populates response parameter with an Array of NSString* with AID names.  Each AID name represent a unique configuration file to be loaded/used when a matching application is found on a card during a transaction.
 
 @param response Returns a NSArray of NSString of AID Names
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to BTPay::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveAIDList:(NSArray**)response;


/**
 * Retrieve Application Data by AID
 *
 Retrieves the configuration information for a provided AID name, if that AID file exists on the terminal.
 
 The TLV data in that AID is returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "9F06"), and the Object being the Value as NSData (example 0xa0000000031010).
 
 The data returned will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 @param AID Name of ApplicationID in ASCII, example "A0000000031020".  Must be between 5 and 16 characters
 @param responseAID  The response returned from the method as a dictionary with Key/Object to match TagValues as follows:
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 
 
 
 */
-(RETURN_CODE) emv_retrieveApplicationData:(NSString*)AID response:(NSDictionary**)responseAID;

/**
 * Retrieve Certificate Authority Public Key
 *
 Retrieves the CAPK as specified by the RID/Index  passed as a parameter in the CAKey structure.  The CAPK will be in the response parameter
 
 @param rid The RID of the key to retrieve
 @param index The Index of the key to retrieve
 @param response Response returned as a CAKey
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPK:(NSString*)rid index:(NSString*)index response:(CAKey**)response;


/**
 * Retrieve Certificate Authority Public Key
 - BTPay 200
 *
 Retrieves the CAPK as specified by the RID/Index  passed as a parameter in the CAKey structure.  The CAPK will be in the response parameter
 
 @param rid The RID of the key to retrieve
 @param index The Index of the key to retrieve
 @param response Response returned as a NSData object with the following data:
 - 5 bytes RID
 - 1 byte Index
 - 1 byte Hash Algorithm
 - 1 byte Encryption Algorithm
 - 20 bytes HashValue
 - 4 bytes Public Key Exponent
 - 2 bytes Modulus Length
 - Variable bytes Modulus>
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 
 */
-(RETURN_CODE) emv_retrieveCAPKFile:(NSString*)rid index:(NSString*)index response:(NSData**)response;

/**
 * Retrieve the Certificate Authority Public Key list
 *
 Returns all the CAPK RID and Index.  Populates response parameter with an array of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.
 
 @param response Response returned contains an NSArray of NSString items, 12 characters each, characters 1-10 RID, characters 11-12 index.  Example "a00000000357" = RID a00000003, Index 57
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveCAPKList:(NSArray**)response;



/**
 * Retrieve the Certificate Revocation List
 
 *
 Returns all the RID in the CRL.
 @param response Response returned as an NSArray of NSData objects 9-bytes each:
 - 5-bytes RID, 1-byte Index, 3-byte Serial Number
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveCRLList:(NSMutableArray**)response;





/**
 * Retrieve Terminal Data
 
 *
 Retrieves the tag values associated with the terminal configuration file.  This will be a combination of uneditable major configuration tags for the kernel configuration (example 9F33, Terminal Capabilities), and editable tags set with IDT_Device::emv_setTerminalData:()  (example DF13, Terminal Action Code - Default)
 
 The TLV data returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "DF13"), and the Object being the Value as NSData (example 0x00058003FF).
 
 The data returned will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 */
-(RETURN_CODE) emv_retrieveTerminalData:(NSDictionary**)responseData;

/**
 * Retrieve Transaction Results
 
 *
 Retrieves the requested tag values (if they exist) from the last transaction.
 
 The TLV data returned as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "5A"), and the Object being the Value as NSData (example 0x41359276429372938).
 
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_retrieveTransactionResult:(NSData*)tags retrievedTags:(NSDictionary**)retrievedTags;

/**
 * Set Application Data by AID
 *
 Sets the configuration information for a provided AID name, with TLV data that populates a NSDictionary.
 
 The TLV data for the AID is sent as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "9F06"), and the Object being the Value as NSData (example 0xa0000000031010).
 
 The data for the AID configuration will will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 NOTES:
 There is no minimum defined set of AID TLV data that must be provided, other than 9F06 for the AID name.
 
 If this AID is selected and matched during an EMV transaction, any data in this AID will either OVERRIDE the same data in the terminal configuration file, or PROVIDE the data if it is non-existant in the terminal configuration file.
 
 AID configuration information is provided during L3 certification.  Dummy/stub AID data can be used pre-certification to test EMV transaction as long as at least tag 9F06 is defined that makes up the AID configuration locator.
 
 There are convenience utilities to turn a TLV NSData object into a NSDictionary, and a NSDictionary into a NSData object in IDTUtility:
 @code
 +(NSDictionary*) TLVtoDICT:(NSData*)param;
 +(NSData*) DICTotTLV:(NSDictionary*)tags;
 @endcode
 
 Also utilities to turn a HEX/ASCII string to NSDATA and back again
 
 @code
 + (NSData *)hexToData:(NSString*)str ;
 +(NSString*) dataToHexString:(NSData*)data;
 @endcode
 
 
 
 @param aidName aidName AID name.  Example "a0000000031010"
 @param data NSDictionary with Tags/Values for the AID configuration file
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setApplicationData:(NSString*)aidName configData:(NSDictionary*)data;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK as specified by the CAKey structure
 
 @param key CAKey containing the RID, Index, and key data to set
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCAPK:(CAKey)key;

/**
 * Set Certificate Authority Public Key
 *
 Sets the CAPK as specified by the CAKey raw format
 
 @param key CAKey format:
 [5 bytes RID][1 byte Index][1 byte Hash Algorithm][1 byte Encryption Algorithm][20 bytes HashValue][4 bytes Public Key Exponent][2 bytes Modulus Length][Variable bytes Modulus]
 Where:
 - Hash Algorithm: The only algorithm supported is SHA-1.The value is set to 0x01
 - Encryption Algorithm: The encryption algorithm in which this key is used. Currently support only one type: RSA. The value is set to 0x01.
 - HashValue: Which is calculated using SHA-1 over the following fields: RID & Index & Modulus & Exponent
 - Public Key Exponent: Actually, the real length of the exponent is either one byte or 3 bytes. It can have two values: 3 (Format is 0x00 00 00 03), or  65537 (Format is 0x00 01 00 01)
 - Modulus Length: LenL LenH Indicated the length of the next field.
 - Modulus: This is the modulus field of the public key. Its length is specified in the field above.
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) emv_setCAPKFile:(NSData*)file;

/**
 * Set Certificate Revocation List
 *
 Sets the CRL list
 
 @param data CRLEntries as a repeating occurance of CRL: CRL1 CRL2 … CRLn.
 CRL format is
 - 5Bytes RID
 - 1Byte CA public key Index
 - 3Bytes Certificate Serial Number
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_UniPayII::device_getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setCRLEntries:(NSData*)data;




/**
 * Set Terminal Data
 
 *
 Sets the terminal configuration information, with TLV data that populates a NSDictionary.
 
 The TLV data for the terminal configuration is sent as a NSDictionary, with the Key being the tag name as a NSString representation of the tag hex value (example "DF13"), and the Object being the Value as NSData (example 0x00080039FF).
 
 The data for the terminal configuration will will be from the range of allowable kernel EMV tags.  Please see "EMV Tag Reference" at the end of this document for the listing.
 
 NOTES:
 There is an uneditable set of tags that make up the current kernel configuration major parameters.  Any attempt to set those will return an error.
 
 If an AID is selected and matched during an EMV transaction, any data in that AID will either OVERRIDE the same data in the terminal configuration file, or PROVIDE the data if it is non-existant in the terminal configuration file.
 
 
 There are convenience utilities to turn a TLV NSData object into a NSDictionary, and a NSDictionary into a NSData object in IDTUtility:
 @code
 +(NSDictionary*) TLVtoDICT:(NSData*)param;
 +(NSData*) DICTotTLV:(NSDictionary*)tags;
 @endcode
 
 Also utilities to turn a HEX/ASCII string to NSDATA and back again
 
 @code
 + (NSData *)hexToData:(NSString*)str ;
 +(NSString*) dataToHexString:(NSData*)data;
 @endcode
 
 
 
 @param data NSDictionary with Tags/Values for the Terminal configuration file
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0002: Invalid Response: invalid response data - RETURN_CODE_ERR_CMD_RESPONSE
 - 0x0003: Timeout: time out for task or CMD - RETURN_CODE_ERR_TIMEDOUT
 - 0x0004: Invalid Parameter: wrong parameter - RETURN_CODE_ERR_INVALID_PARAMETER
 - 0x0005: MSR Busy: SDK is doing MSR or ICC task - RETURN_CODE_SDK_BUSY_MSR
 - 0x0006: PINPad Busy:  SDK is doing PINPad task - RETURN_CODE_SDK_BUSY_PINPAD
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 - 0x0100 through 0xFFFF refer to IDT_Device::getResponseCodeString:()
 
 */
-(RETURN_CODE) emv_setTerminalData:(NSDictionary*)data;





/**
 * Start EMV Transaction Request
 *
 Authorizes the EMV transaction  for an ICC card
 
 The tags will be returned in the emvTransactionData delegate protocol.
 
 
 By default, auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_VP3300::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_VP3300::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP3300::emv_disableAutoAuthenticateTransaction:()
 
 @param amount Transaction amount value  (tag value 9F02)
 @param amtOther Other amount value, if any  (tag value 9F03)
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.  Passed as NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 If tags 9F02 (amount),9F03 (other amount), or 9C (transaction type) are included, they will take priority over these values supplied as individual parameters to this method.
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response
 
 @param forceOnline TRUE = do not allow offline approval,  FALSE = allow ICC to approve offline if terminal capable
 @param autoAuthenticate Will automatically execute Authenticate Transacation after start transaction returns successful
 @param fallback Indicate if it supports fallback to MSR
 
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) emv_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline fallback:(BOOL)fallback;


/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Exchange APDU (unencrypted)
 
 *
 * Sends an APDU packet to the ICC.  If successful, response is returned in APDUResult class instance in response parameter.
 
 @param dataAPDU  APDU data packet
 @param response Unencrypted/encrypted parsed APDU response
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) icc_exchangeAPDU:(NSData*)dataAPDU response:(APDUResponse**)response;
/**
 * Get Reader Status
 *
 Returns the reader status
 
 @param readerStatus Pointer that will return with the ICCReaderStatus results.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 @code
 ICCReaderStatus* readerStatus;
 RETURN_CODE rt = [[IDT_Device sharedController] getICCReaderStatus:&readerStatus];
 if(RETURN_CODE_DO_SUCCESS != rt){
 NSLog(@"Fail");
 }
 else{
 NSString *sta;
 if(readerStatus->iccPower)
 sta =@"[ICC Powered]";
 else
 sta = @"[ICC Power not Ready]";
 if(readerStatus->cardSeated)
 sta =[NSString stringWithFormat:@"%@,[Card Seated]", sta];
 else
 sta =[NSString stringWithFormat:@"%@,[Card not Seated]", sta];
 
 
 @endcode
 */

-(RETURN_CODE) icc_getICCReaderStatus:(ICCReaderStatus**)readerStatus;

/**
 * Power On ICC
 
 
 *
 * Power up the currently selected microprocessor card in the ICC reader
 *
 * @param response Response returned. If Success, ATR String. If Failure, ASCII encoded data of error string
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.
 
 
 
 
 */
-(RETURN_CODE) icc_powerOnICC:(NSData**)response;



/**
 * Power Off ICC
 
 
 *
 * Powers down the ICC
 
 * @param error Returns the error, if any
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.
 
 If Success, empty
 If Failure, ASCII encoded data of error string
 */
-(RETURN_CODE) icc_powerOffICC:(NSString**)error;

/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;

/**
 * Enable MSR Swipe
 
 *
 Enables CLTS and MSR, waiting for swipe or tap to occur. Returns IDTEMVData to deviceDelegate::emvTransactionData:() 
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_VP3300::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;

/**
 *Check if device is connected
 */
-(bool) isConnected;



/**
 * Start A Transaction Request
 *
 Authorizes the EMV, CTLS, or MSR transaction
 
 
 For a contact ICC EMV transaction, by default auto authorize is ENABLED.  If auto authorize is DISABLED, this function will complete with a return of EMV_RESULT_CODE_START_TRANSACTION_SUCCESS to emvTransactionData delegate protocol, and then IDT_UniPayII::emv_authenticateTransaction() must be executed.  If auto authorize is ENABLED (default), IDT_UniPayII::emv_authenticateTransaction() will automatically be executed after receiving the result EMV_RESULT_CODE_START_TRANSACTION_SUCCESS.  The auto authorize can be enabled/disabled with IDT_VP3300::emv_disableAutoAuthenticateTransaction:()
 
 @param amount Transaction amount value  (tag value 9F02)
 @param amtOther Other amount value, if any  (tag value 9F03)
 @param type Transaction type (tag value 9C).
 @param timeout Timeout value in seconds.
 @param tags Any other tags to be included in the request.  Passed as NSData.  Example, tag 9F0C with amount 0x000000000100 would be 0x9F0C06000000000100
 If tags 9F02 (amount),9F03 (other amount), or 9C (transaction type) are included, they will take priority over these values supplied as individual parameters to this method.
 Tag DFEE1A can be used to specify tags to be returned in response, in addition to the default tags. Example DFEE1A049F029F03 will return tags 9F02 and 9F03 with the response
 
 @param forceOnline TRUE = do not allow offline approval,  FALSE = allow ICC to approve offline if terminal capable
 @param autoAuthenticate Will automatically execute Authenticate Transacation after start transaction returns successful
 @param fallback Indicate if it supports fallback to MSR
 
 
 * @return RETURN_CODE:  Values can be parsed with errorCode.getErrorString()
 
 */
-(RETURN_CODE) device_startTransaction:(double)amount amtOther:(double)amtOther type:(int)type timeout:(int)timeout tags:(NSData*)tags forceOnline:(BOOL)forceOnline  fallback:(BOOL)fallback;


@end
