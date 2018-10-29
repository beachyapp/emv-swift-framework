//
//  IDT_UniMag.h
//  IDTech
//
//  Created by Randy Palermo on 2/5/15.
//  Copyright (c) 2015 IDTech Products. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IDTMSRData.h"
#import "APDUResponse.h"
#import "IDT_Device.h"





/** Protocol methods established for IDT_UniMag class  **/
@protocol IDT_UniMag_Delegate <NSObject>

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

@end

@interface IDT_UniMag : NSObject<IDT_Device_Delegate>{
    id<IDT_UniMag_Delegate> delegate;
}

@property(strong) id<IDT_UniMag_Delegate> delegate;  //!<- Reference to IDT_UniMag_Delegate.

/**
 * SDK Version
 - All Devices
 *
 Returns the current version of IDTech.framework
 
 @return  Framework version
 */
+(NSString*) SDK_version;

/**
 * Singleton Instance
 - All Devices
 *
 Establishes an singleton instance of IDT_UniMag class.
 
 @return  Instance of IDT_UniMag
 */
+(IDT_UniMag*) sharedController;

/**
 * Send UniMag Command
 
 *
 * @param command  A command to execute from UNIMAG_COMMAND_Types enumeration
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 @code
 typedef enum{
    UNIMAG_COMMAND_DEFAULT_GENERAL_SETTINGS,
    UNIMAG_COMMAND_ENABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_DISABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_ENABLE_EXP_DATE,
    UNIMAG_COMMAND_DISABLE_EXP_DATE,
    UNIMAG_COMMAND_CLEAR_BUFFER,
    UNIMAG_COMMAND_RESET_BAUD_RATE
 }UNIMAG_COMMAND_Types;
 @endcode
 *
 */
-(RETURN_CODE) device_sendUniMagCommand:(UNIMAG_COMMAND_Types)command;

/**
 * Polls device for Serial Number
 
 *
 * @param response  Returns Serial Number
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) config_getSerialNumber:(NSString**)response;

/**
 * Get Response Code String
 *
 Interpret a IDT_UniMag response code and return string description.
 
 @param errorCode Error code, range 0x0000 - 0xFFFF, example 0x0300
 
 
 * @return Verbose error description
 
 
 */
-(NSString *) device_getResponseCodeString: (int) errorCode;


/**
 * Connect To Audio Reader
 * @return RETURN_CODE
 *
 Attemps to recognize and connect to an IDTech MSR device connected via the audio port.
 
 */

-(RETURN_CODE) device_connectToAudioReader;

/**
 * Disable MSR Swipe
 
 
 
 Cancels MSR swipe request.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 */

-(RETURN_CODE) msr_cancelMSRSwipe;


/**
 * Enable MSR Swipe
 
 *
 Enables MSR, waiting for swipe to occur. Allows track selection. Returns IDTMSRData instance to deviceDelegate::swipeMSRData:()
 
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */

-(RETURN_CODE) msr_startMSRSwipe;


/**
 * Set Swipe Data Encryption
 *
 * Sets the swipe encryption method
 *
 @param encryption 1 = TDES, 2 = AES
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeEncryption:(int)encryption;

/**
 * Set PrePAN Clear Digits
 
 *
 Sets the number of digits to show in clear text at the beginning of PAN
 
 @param clearDigits Amount of characters to display cleartext at beginning of PAN. Valid range 0-6.  Default value 4.
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:())
 
 */
-(RETURN_CODE) device_setPrePANClearDigits:(int)clearDigits;


/**
 * Polls device for next KSN
 
 *
 * @param response  Returns next KSN
 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 *
 */
-(RETURN_CODE) msr_getNextKSN:(NSData**)response;


/**
 * Set Swipe Force Encryption
 *
 * Sets the swipe force encryption options
 *
 @param forceON TRUE = Force Encryption ON,  FALSE = Force Encryption OFF

 
 * @return RETURN_CODE:  Return codes listed as typedef enum in IDTCommon:RETURN_CODE.  Values can be parsed with IDT_UniMag::device_getResponseCodeString:()
 
 
 */
-(RETURN_CODE) msr_setSwipeForcedEncryptionOption:(BOOL)forceON;

/**
 Device Connected
 
 @return isConnected  Boolean indicated if UniMag is connected
 
 */

-(bool) isConnected;

@end
