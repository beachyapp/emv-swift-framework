//
//  IDT_BTMag.h
//  IDTech
//
//  Created by Randy Palermo on 8/28/14.
//  Copyright (c) 2014 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "IDT_Device.h"


typedef enum{
    EPT_NOKEY = 0x30,
    EPT_TDES,
    EPT_AES
}Encryption_Type;

typedef enum{
    PRE_AMBLE =0x02,
    POST_AMBLE
}PostPreamble;

typedef enum{
    BOTH_DIRECTION=0x31,
    HEAD_DIRECTION,
    HEAD_AGAINST,
    RAW_DATA
    
}DecodingMethod;

typedef enum{
    KEY_FIXED=0x30,
    KEY_DUKPT
}KeyManagementType;

typedef enum{
    TRACK_1=0x31,
    TRACK_2,
    TRACK_1_and_2,
    TRACK_3,
    TRACK_1_and_3,
    TRACK_2_and_3,
    TRACK_ALL,
    TRACK_ANY1,
    TRACK_ANY2
}MagneticTrack;

typedef enum{
    TRACK1_ID= 1,
    TRACK2_ID,
    TRACK3_ID
}TrackID;

/** Protocol methods established for IDT_BTPay class  **/
@protocol IDT_BTMag_Delegate <NSObject>

@optional
-(void) deviceConnected; //!<Fires when device connects.  If a connection is established before the delegate is established (no delegate to send initial connection notification to), this method will fire upon establishing the delegate.
-(void) deviceDisconnected; //!<Fires when device disconnects.
- (void) dataInOutMonitor:(NSData*)data  incoming:(BOOL)isIncoming; //!<All incoming/outgoing data going to the device can be monitored through this delegate.
//!< @param data The serial data represented as a NSData object
//!< @param isIncoming The direction of the data
//!<- <c>TRUE</c> specifies data being received from the device,
//!<- <c>FALSE</c> indicates data being sent to the device.

- (void) swipeMSRData:(IDTMSRData*)cardData;//!<Receives card data from MSR swipe.
//!< @param cardData Captured card data from MSR swipe

@end

/**
 Class to drive the IDT_BTMag device
 */
@interface IDT_BTMag : NSObject<IDT_Device_Delegate>{
    id<IDT_BTMag_Delegate> delegate;
}

@property(strong) id<IDT_BTMag_Delegate> delegate;  //!<- Reference to IDT_BTPay_Delegate.



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
 Establishes an singleton instance of IDT_BTMag class.
 
 @return  Instance of IDT_BTMag
 */
+(IDT_BTMag*) sharedController;



/**
 * Sets the OSX Connection Method
 *
 When using BTPay on OSX, the device can connect either via Bluetooth or USB-HID.  Default is Bluetooth.  Use this function to change the connection method to USB-HID
 
 @param isUSB  TRUE = connect via USB-HID, FALSE = connect via BlueTooth.
 
 
 */
+(void) connectUSB:(BOOL)isUSB;


/**
 * Polls device for Firmware Version
 
 *
 * @param response Response returned of Firmware Version
 *
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniPay::device_getResponseCodeString:()
 *
 */
-(RETURN_CODE) device_getFirmwareVersion:(NSString**)response;

/**
 * Send a NSData object to device
 *
 * Sends a command represented by the provide NSData object to the device through the accessory protocol.
 *
 * @param cmd NSData representation of command to execute
 * @param lrc If <c>TRUE</c>, this will wrap command with start/end/CheckLRC:  '{STX} data {ETX} {CheckLRC}'
 @param response Response data
 
 * @return RETURN_CODE:
 - 0x0000: Success: no error - RETURN_CODE_DO_SUCCESS
 - 0x0001: Disconnect: no response from reader - RETURN_CODE_ERR_DISCONNECT
 - 0x0007: Unknown:  Unknown error - RETURN_CODE_ERR_OTHER
 */
-(RETURN_CODE) device_sendDataCommand:(NSData*)cmd calcLRC:(BOOL)lrc response:(NSData**)response;

/**
 Is Device Connected
 
 Returns the connection status of the BTMag
 
 */
-(bool) isConnected;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniMag response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;



/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 *Close Device
 */

-(void) close;

@end
