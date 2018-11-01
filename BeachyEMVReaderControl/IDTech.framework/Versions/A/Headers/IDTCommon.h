//
//  IDTCommon.h
//  IDTech
//
//  Created by Randy Palermo on 5/4/15.
//  Copyright (c) 2015 IDTech Products. All rights reserved.
//



/** Structure used to return response from IDT_BTPay::icc_getICCReaderStatus() and IDT_UniPay::icc_getICCReaderStatus()
 */
typedef struct {
    bool iccPower; //!< Determines if ICC has been powered up
    bool cardSeated;  //!< Determines if card is inserted
    bool latchClosed;  //!< Determines if Card Latch is engaged.  If device does not have a latch, value is always FALSE
    bool cardPresent;  //!< If device has a latch, determines if the card is present in device.  If the device does not have a latch, value is always FALSE
    bool magneticDataPresent;  //!< True = Magnetic data present, False = No Magnetic Data
}ICCReaderStatus;


/**
 Structure to set ICC power on options.  Used by IDT_BTPay::icc_powerOnICC:response:() IDT_UniPay::icc_powerOnICC:response:()
 */
typedef struct {
    BOOL sendIFS; //!< Send S(IFS) request if T=1 protocolError: Reference source not found
    BOOL explicitPPS;  //!< Explicit PPSError: Reference source not found
    BOOL disableAutoPPS; //!< No auto pps for negotiate mode
    BOOL disableResponseCheck;  //!< No check on response of S(IFS) request
    unsigned char* pps; //!< pps is used to set the Protocol and Parameters Selection between card and reader, only Di <= 4 are supported. pps must follow the structure specified in ISO 7816-3 as PPS0, [PPS1], [PPS2], and [PPS3]. For more information see ISO 7816-3 section 7.2.
    unsigned char ppsLength; //!< lenght of pps data
}PowerOnStructure;
#ifndef UNIPAY_SHOULD_SKIP_THIS

/**
 Certificate Authority Public Key
 
 Used as parameter in IDT_BTPay::emv_retrieveCAPK:response:(), IDT_BTPay::emv_removeCAPK:(), IDT_BTPay::emv_setCAPK:(), IDT_UniPay::emv_retrieveCAPK:response:(), IDT_UniPay::emv_removeCAPK:(), IDT_UniPay::emv_setCAPK:()
 
 Used as return value in IDT_BTPay::emv_retrieveCAPK:response:() IDT_UniPay::emv_retrieveCAPK:response:()
 */
typedef struct {
    unsigned char hashAlgorithm;       //!< Hash Algorithm  0x01 = SHA-1
    unsigned char encryptionAlgorithm;       //!< Encryption Algorithm 0x01 = RSA
    unsigned char rid[5];  //!< As per payment networks definition
    unsigned char index;  //!< As per payment networks definition
    unsigned char exponentLength;  //!< Length of exponent. 0x01 or 0x03 as per EMV specs
    unsigned char keyLength;  //!< Length of key. max 248 bytes as per EMV specs
    unsigned char exponent[3];  //!< CA Public Key Exponent
    unsigned char key[248];       //!< CA Public Key
    
} CAKey;


/**
 AID Entry - Used to populate array in IDT_BTPay::emv_retrieveAIDList:()  IDT_UniPay::emv_retrieveAIDList:().
 */
typedef struct {
    unsigned char aid[16];        //!< AID value as per payment networks.
    unsigned char aidLen;        //!< AID’s length.
} AIDEntry;

/**
 Mask and Encryption - Used to Set/Retrieve mask and encryption values IDT_BTPay::emv_retrieveAIDList:()  IDT_UniPay::emv_retrieveAIDList:().
 */
typedef struct {
    unsigned char prePANClear;        //!< Leading PAN digits to display. Values '0' - '6'.  Default '4'
    unsigned char postPANClear;        //!< Last PAN digits to display. Values '0' - '4'.  Default '4'
    unsigned char maskChar;        //!< Last PAN digits to display. Values 0x20-0x7E.  Default 0x2A '*'
    unsigned char displayExpDate;        //!< Mask or display expiration date. Values '0' = mask, '1' = don't mask.  Default '1'
    unsigned char baseKeyType;        //!< BTPay Only. Key Type. Values '0' = Data Key,  '1' = Pin Key.  Default '0'
    unsigned char encryptionType;     //!< BTPay Only. Key Type. Values '1' = TDES,  '2' = AES.  Default '1'
    unsigned char encryptionOption;     //!< UniPay II Only.
    //!< Bit 0: T1 force encrypt
    //!< Bit 1 : T2 force encrypt
    //!< Bit 2 : T3 force encrypt
    //!< Bit3 : T3 force encrypt when card type is 0
    unsigned char maskOption;     //!< UniPay II Only.
    //!< Bit0: T1 mask allowed
    //!< Bit1: T2 mask allowed
    //!< Bit2: T3 mask allowed
} MaskAndEncryption;


/**
 device AID File - 571 bytes
 
 Used as parameter in IDT_BTPay::emv_setApplicationData:()
 
 Used as return value of aidResponse in IDT_BTPay::emv_retrieveApplicationData:response:()
 */
typedef struct {
    unsigned char acquirerIdentifier[6];        //!< Indicates  which  acquirer/processor  processes  the corresponding AID. Tag 9F01
    unsigned char aid[16];        //!< AID value as per payment networks.  Tag 9F06
    unsigned char aidLen;        //!< AID’s length.
    unsigned char applicationSelectionIndicator;        //!< Standard parameter.
    unsigned char applicationVersionNumber[2];        //!< EMV application version number.   Tag 9F09
    unsigned char XAmount[3];        //!< Not used by Agnos Framework.
    unsigned char YAmount[3];        //!< Not used by Agnos Framework.
    unsigned char skipTACIACDefault;        //!< Indicates whether or not terminal uses default values for risk management.
    unsigned char tac;        //!< Indicates whether or not terminal uses Terminal Action Code.   0x00 or 0x01
    unsigned char floorlLimitChecking;        //!< Indicates whether  or  not  terminal  uses  Floor  Limit Checking.   0x00 or 0x01
    unsigned char randomTransactionSelection;        //!< Indicates whether  or  not  terminal  uses  Random Transaction Selection.   0x00 or 0x01
    unsigned char velocitiyChecking;        //!< Indicates whether  or  not  terminal  uses  Velocity Checking.   0x00 or 0x01
    unsigned char tACDenial[5];        //!< Terminal Action Code Denial.
    unsigned char tACOnline[5];        //!< Terminal Action Code Online.
    unsigned char tACDefault[5];        //!< Terminal Action Code Default.
    unsigned char terminalFloorLimit[3];        //!< Standard parameter.  Tag 9F1B
    unsigned char targetPercentage;        //!< EMV offline risk management parameter.
    unsigned char thresholdValue[3];        //!< EMV offline risk management parameter.
    unsigned char maxTargetPercentage;        //!< EMV offline risk management parameter.
    unsigned char defaultTDOL;        //!< Standard parameter.
    unsigned char tdolValue[252];        //!< Transaction Data Object List value.
    unsigned char tdolLen;        //!< Transaction Data Object List length.
    unsigned char defaultDDOL;        //!< Standard  parameter..  Tag
    unsigned char ddolValue[252];        //!< Dynamic Data Object List value.
    unsigned char ddolLen;        //!< Dynamic Data Object List length.
    unsigned char transactionCurrencyCode[2];        //!< AID’s currency.   Example: For Canada, {0x01,0x24}.  Tag 5F2A
    unsigned char transactionCurrencyExponent;        //!< Transaction Currency Exponent.   Example: Amount 4.53$ is managed as 453. Tag 5F36
    
} ApplicationID;


/**
 device Terminal Configuration File - 44 bytes
 
 Used as parameter in IDT_BTPay::setTerminalData:()
 
 Used as return value in IDT_BTPay::emv_retrieveTerminalData:()
 */
typedef struct {
    unsigned char terminalCountryCode[2];     //!< Terminal’s location. Tag 9F1A {0x08,0x40}
    unsigned char provideCardholderConfirmation;     //!< Indicates whether or not cardholder may confirm application selection at EMV Selection time. Tag 58 0x00 or 0x01
    unsigned char terminalType;     //!< Standard parameter. Tag 9F35  See EMVCo book IV
    unsigned char emvContact;     //!< Indicates whether terminal supports EMV contact. Tag 9F33, byte 1, bit 6 0x00 or 0x01
    unsigned char terminalCapabilities[3];     //!< Standard parameter.  Tag 9F33 See EMVCo book IV
    unsigned char additionalTerminalCapabilities[5];     //!< Standard parameter.  Tag 9F40 See EMVCo book IV
    unsigned char emvContactless;     //!< Indicates whether  or  not  terminal support scontactless  in EMV mode.  0x00 or 0x01
    unsigned char magstripe;     //!< Indicates whether terminal supports magstripe.  0x00 or 0x01
    unsigned char pinTimeOut;     //!< In seconds. Time allocated to cardholder to enter PIN.  Binary value Example  : 0x0F for 15s
    unsigned char batchManaged;     //!< Indicates whether or not Batch messages are supported by Terminal.  0x00 or 0x01
    unsigned char adviceManaged;     //!< Indicates whether or not Advice messages are supported by Terminal (only if needed by Level3 implementation).  0x00 or 0x01
    unsigned char pse;     //!< Indicates whether or not PSE Selection method is supported by Terminal.  0x00 or 0x01
    unsigned char autoRun;     //!< Indicates whether or not Terminal is configured in AutoRun.  0x00 or 0x01
    unsigned char predefinedAmount[3];     //!< Fixed amount.  Binary value
    unsigned char pinByPass;     //!< Indicates whether or not PIN bypass is supported by Terminal.  0x00 or 0x01
    unsigned char referalManaged;     //!< Indicates whether or not Referal managed are supported by Terminal (only if needed by Level3 implementation)..  0x00 or 0x01
    unsigned char defaultTAC;     //!< Indicates whether or not Default TAC are supported by Terminal.  0x00 or 0x01
    unsigned char defaultTACDenial[5];     //!< Default TAC Denial value.  See EMVCo book IV
    unsigned char defaultTACOnline[5];     //!< Default TAC Online value.  See EMVCo book IV
    unsigned char defaultTACDefault[5];     //!< Default TAC Default value.  See EMVCo book IV
    unsigned char notRTS;     //!< Indicates RTS are not supported by Terminal or not.  0x00 or 0x01
    unsigned char notVelocity;     //!< Indicates Velocity are not supported by Terminal or not.  0x00 or 0x01
    unsigned char cdaType;  //!< Supported CDA type. Value should be 0x02
} TerminalData;

/**
 Certificate Revocation List Entry - 9 bytes
 
 Used as parameter in IDT_BTPay::emv_retrieveCRLForRID:response:(), IDT_BTPay::emv_removeCRL:(), IDT_BTPay::emv_removeCRLUnit:(), IDT_BTPay::emv_setCRL:() IDT_UniPay::emv_retrieveCRLForRID:response:(), IDT_UniPay::emv_removeCRL:(), IDT_UniPay::emv_removeCRLUnit:(), IDT_UniPay::emv_setCRL:()
 
 */
typedef struct {
    unsigned char rid[5];  //!< As per payment networks definition
    unsigned char index;  //!< As per payment networks definition
    unsigned char serialNumber[3];  //!< As per payment networks definition
} CRLEntry;

#endif
//Versioning

//! Capture Encode Types
typedef enum _CAPTURE_ENCODE_TYPE{
    CAPTURE_ENCODE_TYPE_ISOABA=0,
    CAPTURE_ENCODE_TYPE_AAMVA=1,
    CAPTURE_ENCODE_TYPE_Other=3,
    CAPTURE_ENCODE_TYPE_Raw=4,
    CAPTURE_ENCODE_TYPE_JIS_II=5,
    CAPTURE_ENCODE_TYPE_JIS_I=6,
    CAPTURE_ENCODE_TYPE_MANUAL_ENTRY=7
} CAPTURE_ENCODE_TYPE;

/** Capture Encrypt Types **/
typedef enum{
    CAPTURE_ENCRYPT_TYPE_TDES=0,
    CAPTURE_ENCRYPT_TYPE_AES=1,
    CAPTURE_ENCRYPT_TYPE_NO_ENCRYPTION=99
} CAPTURE_ENCRYPT_TYPE;

typedef enum{
    POWER_ON_OPTION_IFS_FLAG=1,
    POWER_ON_OPTION_EXPLICIT_PPS_FLAG=2,
    POWER_ON_OPTION_AUTO_PPS_FLAG=64,
    POWER_ON_OPTION_IFS_RESPONSE_CHECK_FLAG=128
}POWER_ON_OPTION;

typedef enum{
    LANGUAGE_TYPE_ENGLISH=1,
    LANGUAGE_TYPE_PORTUGUESE,
    LANGUAGE_TYPE_SPANISH,
    LANGUAGE_TYPE_FRENCH
}LANGUAGE_TYPE;

typedef enum{
    PIN_KEY_TDES_MKSK_extp=0x00, //external plain text
    PIN_KEY_TDES_DUKPT_extp=0x01, //external plain text
    PIN_KEY_TDES_MKSK_intl=0x10,  //internal BTPay
    PIN_KEY_TDES_DUKPT_intl=0x11, //internal BTPay
    PIN_KEY_TDES_MKSK2_intl=0x20,  //internal UniPayII
    PIN_KEY_TDES_DUKPT2_intl=0x21, //internal  UniPayII
}PIN_KEY_Types;

#define EVENT_IDLE  0
#define EVENT_ASYNC_DATA 0x0100
#define EVENT_SDK_BUSY 0x0200

typedef enum{
    EVENT_PINPAD_UNKNOWN = 11,
    EVENT_PINPAD_ENCRYPTED_PIN,
    EVENT_PINPAD_NUMERIC,
    EVENT_PINPAD_AMOUNT,
    EVENT_PINPAD_ACCOUNT,
    EVENT_PINPAD_ENCRYPTED_DATA,
    EVENT_PINPAD_CANCEL,
    EVENT_PINPAD_TIMEOUT,
    EVENT_PINPAD_FUNCTION_KEY,
    EVENT_PINPAD_DATA_ERROR,
    EVENT_PINPAD_PAN_ERROR,
    EVENT_PINPAD_PIN_DUKPT_MISSING,
    EVENT_PINPAD_PIN_DUKPT_EXHAUSTED,
    EVENT_PINPAD_DISPLAY_MESSAGE_ERROR
}EVENT_PINPAD_Types;



typedef enum{
    IDT_DEVICE_BTPAY_IOS = 0,
    IDT_DEVICE_BTPAY_OSX_BT,
    IDT_DEVICE_BTPAY_OSX_USB,
    IDT_DEVICE_UNIPAY_IOS,
    IDT_DEVICE_UNIPAY_OSX_USB,
    IDT_DEVICE_UNIPAYII_IOS,
    IDT_DEVICE_UNIPAYII_OSX_USB,
    IDT_DEVICE_IMAG_IOS,
    IDT_DEVICE_VP3300_IOS,
    IDT_DEVICE_VP3300_OSX_USB,
    IDT_DEVICE_UNIMAG,
    IDT_DEVICE_BTMAG_IOS,
    IDT_DEVICE_BTMAG_OSX_BT,
    IDT_DEVICE_BTMAG_OSX_USB,
    IDT_DEVICE_UNIPAYI_V_IOS,
    IDT_DEVICE_UNIPAYI_V_OSX_USB,
    IDT_DEVICE_NEO2_IOS


}IDT_DEVICE_Types;



typedef enum{
    EVENT_MSR_UNKNOWN = 31,
    EVENT_MSR_CARD_DATA,
    EVENT_MSR_CANCEL_KEY,
    EVENT_MSR_BACKSPACE_KEY,
    EVENT_MSR_ENTER_KEY,
    EVENT_MSR_DATA_ERROR,
    EVENT_MSR_ICC_START,
    EVENT_BTPAY_CARD_DATA,
    EVENT_UNIPAYII_EMV_NO_ICC_MSR_DATA,
    EVENT_UNIPAYII_EMV_FALLBACK_DATA,
    EVENT_UNIPAY_KEYLOADING,
    EVENT_MSR_TIMEOUT
}EVENT_MSR_Types;

typedef enum{
    EVENT_ACTIVE_TRANSACTION = 51
}EVENT_CTLS_Types;

typedef enum{
    UNIMAG_COMMAND_DEFAULT_GENERAL_SETTINGS,
    UNIMAG_COMMAND_ENABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_DISABLE_ERR_NOTIFICATION,
    UNIMAG_COMMAND_ENABLE_EXP_DATE,
    UNIMAG_COMMAND_DISABLE_EXP_DATE,
    UNIMAG_COMMAND_CLEAR_BUFFER,
    UNIMAG_COMMAND_RESET_BAUD_RATE
}UNIMAG_COMMAND_Types;

typedef enum {
    RETURN_CODE_DO_SUCCESS = 0,             //!< no error, beginning task
    RETURN_CODE_ERR_DISCONNECT = 0xFF01,         //!< no response from reader
    RETURN_CODE_ERR_CMD_RESPONSE = 0xFF02,       //!< invalid response data
    RETURN_CODE_ERR_TIMEDOUT = 0xFF03,           //!< time out for task or CMD
    RETURN_CODE_ERR_INVALID_PARAMETER = 0xFF04,  //!< wrong parameter
    RETURN_CODE_SDK_BUSY_MSR = 0xFF05,           //!< SDK is doing MSR or ICC task
    RETURN_CODE_SDK_BUSY_PINPAD = 0xFF06,        //!< SDK is doing PINPad task
    RETURN_CODE_SDK_BUSY_CTLS = 0xFF07,        //!< SDK is doing CTLS task
    RETURN_CODE_ERR_OTHER = 0xFF08,              //!< SDK is doing Other task
    RETURN_CODE_FAILED = 0xFF09,                 //!< err response or data
    RETURN_CODE_NOT_ATTACHED = 0xFF0A,           //!< no reader attached
    RETURN_CODE_MONO_AUDIO = 0xFF0B,           //!< mono audio is enabled
    RETURN_CODE_CONNECTED = 0xFF0C,           //!< did connection
    RETURN_CODE_LOW_VOLUME = 0xFF0D,           //!< audio volume is too low
    RETURN_CODE_CANCELED = 0xFF0E,           //!< task or CMD be canceled
    RETURN_CODE_INVALID_STR = 0xFF0F,           //!< UF wrong string format
    RETURN_CODE_NO_FILE = 0xFF10,           //!< UF file not found
    RETURN_CODE_INVALID_FILE = 0xFF11,           //!< UF wrong file format
    RETURN_CODE_HOST_UNREACHABLE = 0xFF12,           //!< Attempt to contact online host failed
    RETURN_CODE_RKI_FAILURE = 0xFF13,           //!< Attempt to perform RKI failed
    RETURN_CODE_SDK_BUSY_CMD = 0xFF14,           //!< SDK is busy processing another CMD

    
    
    RETURN_CODE_EMV_AUTHORIZATION_ACCEPTED = 0x0E00,          //!< Authorization Accepted
    RETURN_CODE_EMV_AUTHORIZATION_UNABLE_TO_GO_ONLINE = 0x0E01,   //!< Unable to go online
    RETURN_CODE_EMV_AUTHORIZATION_TECHNICAL_ISSUE = 0x0E02,   //!< Technical Issue
    RETURN_CODE_EMV_AUTHORIZATION_DECLINED = 0x0E03,           //!< Declined
    RETURN_CODE_EMV_AUTHORIZATION_ISSUER_REFERRAL = 0x0E04,           //!< Issuer Referral transaction
    
    RETURN_CODE_EMV_APPROVED = 0x0F00,   //!< Accept the online transaction
    RETURN_CODE_EMV_DECLINED = 0x0F01,   //!< Decline the online transaction
    RETURN_CODE_EMV_GO_ONLINE = 0x0F02,          //!< Request to go online
    RETURN_CODE_EMV_FAILED = 0x0F03,             //!< Transaction is terminated
    RETURN_CODE_EMV_SYSTEM_ERROR = 0x0F05,       //!< Application was not selected by kernel or ICC format error or ICC missing data error
    RETURN_CODE_EMV_NOT_ACCEPTED = 0x0F07,       //!< ICC didn't accept transaction
    RETURN_CODE_EMV_FALLBACK = 0x0F0A,           //!< Application may fallback to magstripe technology
    RETURN_CODE_EMV_CANCEL = 0x0F0C,             //!< Transaction was cancelled
    RETURN_CODE_EMV_TIMEOUT = 0x0F0D,             //!< Timeout
    RETURN_CODE_EMV_OTHER_ERROR = 0x0F0F,        //!< Other EMV Error
    RETURN_CODE_EMV_OFFLINE_APPROVED = 0x0F10,   //!< Accept the offline transaction
    RETURN_CODE_EMV_OFFLINE_DECLINED = 0x0F11,   //!< Decline the offline transaction
    
    
    
    RETURN_CODE_EMV_NEW_SELECTION = 0x0F21,      //!< ICC detected that the conditions of use are not satisfied
    RETURN_CODE_EMV_NO_AVAILABLE_APPS = 0x0F22,   //!< No app were found on card matching terminal configuration
    RETURN_CODE_EMV_NO_TERMINAL_FILE = 0x0F23,   //!< Terminal file does not exist
    RETURN_CODE_EMV_NO_CAPK_FILE = 0x0F24,       //!< CAPK file does not exist
    RETURN_CODE_EMV_NO_CRL_ENTRY = 0x0F25,       //!< CRL Entry does not exist
    RETURN_CODE_BLOCKING_DISABLED = 0x0FFE,        //!< Return code when blocking is disabled
    RETURN_CODE_COMMAND_UNAVAILABLE = 0x0FFF,       //!< Return code when command is unavailable
    
    //IDG Return Codes
    RETURN_CODE_NEO_SUCCESS = 0xEE00,      //!<Command Successful
    RETURN_CODE_NEO_INCORRECT_HEADER_TAG = 0xEE01,      //!<Incorrect Header Tag
    RETURN_CODE_NEO_UNKNOWN_COMMAND = 0xEE02,      //!<Unknown Command
    RETURN_CODE_NEO_UNKNOWN_SUB_COMMAND = 0xEE03,      //!<Unknown Sub-Command
    RETURN_CODE_NEO_CRC_ERROR_IN_FRAME = 0xEE04,      //!<CRC Error in Frame
    RETURN_CODE_NEO_INCORRECT_PARAMETER = 0xEE05,      //!<Incorrect Parameter
    RETURN_CODE_NEO_PARAMETER_NOT_SUPPORTED = 0xEE06,      //!<Parameter Not Supported
    RETURN_CODE_NEO_MAL_FORMATTED_DATA = 0xEE07,      //!<Mal-formatted Data
    RETURN_CODE_NEO_TIMEOUT = 0xEE08,      //!<Timeout
    RETURN_CODE_NEO_FAILED_NAK = 0xEE0A,      //!<Failed / NACK
    RETURN_CODE_NEO_COMMAND_NOT_ALLOWED = 0xEE0B,      //!<Command not Allowed
    RETURN_CODE_NEO_SUB_COMMAND_NOT_ALLOWED = 0xEE0C,      //!<Sub-Command not Allowed
    RETURN_CODE_NEO_BUFFER_OVERFLOW = 0xEE0D,      //!<Buffer Overflow (Data Length too large for reader buffer)
    RETURN_CODE_NEO_USER_INTERFACE_EVENT = 0xEE0E,      //!<User Interface Event
    RETURN_CODE_NEO_COMM_TYPE_NOT_SUPPORTED = 0xEE11,      //!<Communication type not supported, VT-1, burst, etc.
    RETURN_CODE_NEO_SECURE_INTERFACE_NOT_FUNCTIONAL = 0xEE12,      //!<Secure interface is not functional or is in an intermediate state.
    RETURN_CODE_NEO_DATA_FIELD_NOT_MOD8 = 0xEE13,      //!<Data field is not mod 8
    RETURN_CODE_NEO_PADDING_UNEXPECTED = 0xEE14,      //!<Pad 0x80 not found where expected
    RETURN_CODE_NEO_INVALID_KEY_TYPE = 0xEE15,      //!<Specified key type is invalid
    RETURN_CODE_NEO_CANNOT_RETRIEVE_KEY = 0xEE16,      //!<Could not retrieve key from the SAM (InitSecureComm)
    RETURN_CODE_NEO_HASH_CODE_ERROR = 0xEE17,      //!<Hash code problem
    RETURN_CODE_NEO_CANNOT_STORE_KEY = 0xEE18,      //!<Could not store the key into the SAM (InstallKey)
    RETURN_CODE_NEO_FRAME_TOO_LARGE = 0xEE19,      //!<Frame is too large
    RETURN_CODE_NEO_RESEND_COMMAND = 0xEE1A,      //!<Unit powered up in authentication state but POS must resend the InitSecureComm command
    RETURN_CODE_NEO_EEPROM_NOT_INITALIZED = 0xEE1B,      //!<The EEPROM may not be initialized because SecCommInterface does not make sense
    RETURN_CODE_NEO_PROBLEM_ENCODING_APDU = 0xEE1C,      //!<Problem encoding APDU
    RETURN_CODE_NEO_UNSUPPORTED_INDEX = 0xEE20,      //!<Unsupported Index (ILM). SAM Transceiver error – problem communicating with the SAM (Key Mgr)
    RETURN_CODE_NEO_UNEXPECTED_SEQUENCE_COUNTER = 0xEE21,      //!<Unexpected Sequence Counter in multiple frames for single bitmap (ILM). Length error in data returned from the SAM (Key Mgr)
    RETURN_CODE_NEO_IMPROPER_BITMAP = 0xEE22,      //!<Improper bit map (ILM)
    RETURN_CODE_NEO_REQUEST_ONLINE_AUTHORIZATION = 0xEE23,      //!<Request Online Authorization
    RETURN_CODE_NEO_RAW_DATA_READ_SUCCESSFUL = 0xEE24,      //!<ViVOCard3 raw data read successful
    RETURN_CODE_NEO_MESSAGE_NOT_AVAILABLE = 0xEE25,      //!<Message index not available (ILM). ViVOcomm activate transaction card type (ViVOcomm)
    RETURN_CODE_NEO_VERSION_INFORMATION_MISMATCH = 0xEE26,      //!<Version Information Mismatch (ILM)
    RETURN_CODE_NEO_NOT_SENDING_COMMANDS = 0xEE27,      //!<Not sending commands in correct index message index (ILM)
    RETURN_CODE_NEO_TIMEOUT_ILM = 0xEE28,      //!<Time out or next expected message not received (ILM)
    RETURN_CODE_NEO_ILM_NOT_AVAILABLE = 0xEE29,      //!<ILM languages not available for viewing (ILM)
    RETURN_CODE_NEO_OTHER_LANG_NOT_SUPPORTED = 0xEE2A,      //!<Other language not supported (ILM)
    RETURN_CODE_NEO_KEY_ERROR_41 = 0xEE41,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_42 = 0xEE42,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_43 = 0xEE43,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_44 = 0xEE44,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_45 = 0xEE45,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_46 = 0xEE46,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_47 = 0xEE47,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_48 = 0xEE48,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_49 = 0xEE49,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4A = 0xEE4A,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4B = 0xEE4B,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4C = 0xEE4C,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4D = 0xEE4D,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4E = 0xEE4E,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_KEY_ERROR_4F = 0xEE4F,      //!<Module-specific errors for Key Manager
    RETURN_CODE_NEO_AUTO_SWITCH_OK = 0xEE50,      //!Auto-Switch OK
    RETURN_CODE_NEO_AUTO_SWITCH_FAILED = 0xEE51,      //!Auto-Switch failed
    RETURN_CODE_DATA_DOES_NOT_EXIST = 0xEE60,      //!Data not exist
    RETURN_CODE_DATA_FULL = 0xEE61,      //!Data Full
    RETURN_CODE_WRITE_FLASH_ERROR = 0xEE62,      //!Write Flash Error
    RETURN_CODE_OK_NEXT_COMMAND = 0xEE63,      //!Ok and Have Next Command
    
    RETURN_CODE_CANNOT_START_CONTACT_EMV = 0xEE80,      //!Cannot start Contact EMV transaction
    RETURN_CODE_CTLS_MSR_CANCELLED_BY_CARD_INSERT = 0xEE81,      //!CTLS/MSR cancelled due to card insertion

    
    RETURN_CODE_ACCT_DUKPT_KEY_NOT_EXIST = 0xEE90,      //!Account DUKPT Key not exist
    RETURN_CODE_ACCT_DUKPT_KEY_EXHAUSTED = 0xEE91,      //!Account DUKPT Key KSN exhausted
    
    
    
    RETURN_CODE_NO_SERIAL_NUMBER = 0x6200,      //!No Serial Number
    RETURN_CODE_INVALID_COMMAND = 0x6900,      //!Invalid Command
    RETURN_CODE_NO_ADMIN_DUKPT_KEY = 0x5500,      //!No Admin DUKPT Key
    RETURN_CODE_DUKPT_KEY_STOP = 0x5501,      //!Admin DUKPT Key STOP
    RETURN_CODE_DUKPT_KEY_KSN_IS_ERROR = 0x5502,      //!Admin DUKPT Key KSN is Error
    RETURN_CODE_GET_AUTH_CODE1_FAILED = 0x5503,      //!Get Authentication Code1 Failed
    RETURN_CODE_VALIDATE_AUTH_CODE_ERROR = 0x5504,      //!Validate Authentication Code Error
    RETURN_CODE_DECRYPT_DATA_FAILED = 0x5505,      //!Encrypt Or Decrypt data failed
    RETURN_CODE_NOT_SUPPORT_NEW_KEY_TYPE = 0x5506,      //!Not Support the New Key Type
    RETURN_CODE_NEW_KEY_INDEX_IS_ERROR = 0x5507,      //!New Key Index is Error
    RETURN_CODE_STEP_ERROR = 0x5508,      //!Step Error
    RETURN_CODE_TIMED_OUT = 0x5509,      //!Timed out
    RETURN_CODE_MAC_CHECKING_ERROR = 0x550A,      //!MAC checking error
    RETURN_CODE_KEY_USAGE_ERROR = 0x550B,      //!Key Usage Error
    RETURN_CODE_MODE_OF_USE_ERROR = 0x550C,      //!Mode of Use Error
    RETURN_CODE_ALGORITHM_ERROR = 0x550D,      //!Algorithm Error
    RETURN_CODE_OTHER_ERROR = 0x550F,      //!Other Error
    
    RETURN_CODE_CANNOT_AUTHORIZE_RKI = 0x8001,      //!Authorization: Cannot initialize RKI; no customer/key information found
    RETURN_CODE_NO_KEY_INJECTION_ESTABLISHED = 0x8101,      //!Step 1: No key injection established
    RETURN_CODE_FAILED_TO_ENCRYPT_CHALLENGE = 0x8102,      //!Step 1: Failed to encrypt challenge
    RETURN_CODE_CHALLENGE_LENGTH_INCORRECT = 0x8103,      //!Step 1: challenge length is incorrect
    RETURN_CODE_INCORRECT_CHALLENGE_DATA_STEP1 = 0x8104,      //!Step 1: Incorrect challenge data
    RETURN_CODE_RESPONSE_LENGTH_INCORRECT = 0x8105,      //!Step 1: Response length incorrect
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP1 = 0x8106,      //!Step 1: Firmware responded NAK for Step 1
    RETURN_CODE_KEY_ID_NOT_FOUND_IN_DB = 0x8201,      //!Step 2: Customer key id could not be found in the DB
    RETURN_CODE_KEY_SLOT_DOES_NOT_EXIST = 0x8202,      //!Step 2: Key Slot does not exist
    RETURN_CODE_NO_FUTURE_KSI_FROM_SERVER = 0x8203,      //!Step 2: Could not get the future KSI from the server
    RETURN_CODE_NO_TR31_DATA_BLOCK = 0x8204,      //!Step 2: Could not get TR31 data block
    RETURN_CODE_TR31_BLOCK_LENGTH_INCORRECT = 0x8205,      //!Step 2: TR31 block length is incorrect
    RETURN_CODE_INCORRECT_CHALLENGE_DATA_STEP2 = 0x8206,      //!Step 2: Incorrect challenge data
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP2 = 0x8207,      //!Step 2: Firmware responded NAK for Step 2
    RETURN_CODE_NO_KEY_INJECTION_RECORD = 0x8301,      //!Step 3: No key injection record found
    RETURN_CODE_RKI_FAILED = 0x8302,      //!Step 3: Remote Key Injection failed (NAK)
    RETURN_CODE_INCORRECT_RESPONSE_FORM = 0x8303,      //!Step 3: Incorrect response form
    RETURN_CODE_FIRMWARE_RESPOND_NAK_STEP3 = 0x8304      //!Step 3: Firmware responded NAK for Step 3
    
} RETURN_CODE;


typedef enum {
    DUKPT_KEY_MSR = 0x00,
    DUKPT_KEY_ICC = 0x01,
    DUKPT_KEY_Admin = 0x10,
    DUKPT_KEY_Pairing_PinPad = 0x20,
} DUKPT_KEY_Type;

typedef enum{
    EMV_PIN_MODE_CANCEL = 0X00,
    EMV_PIN_MODE_ONLINE_PIN_DUKPT = 0X01,
    EMV_PIN_MODE_ONLINE_PIN_MKSK = 0X02,
    EMV_PIN_MODE_OFFLINE_PIN = 0X03
} EMV_PIN_MODE_Types;

typedef enum{
    EMV_RESULT_CODE_NO_RESPONSE = -1,
    EMV_RESULT_CODE_APPROVED = 0X00,
    EMV_RESULT_CODE_DECLINED = 0X01,
    EMV_RESULT_CODE_GO_ONLINE = 0X02,
    EMV_RESULT_CODE_FAILED = 0X03,
    EMV_RESULT_CODE_SYSTEM_ERROR = 0X05,
    EMV_RESULT_CODE_NOT_ACCEPT = 0X07,
    EMV_RESULT_CODE_FALLBACK = 0X0A,
    EMV_RESULT_CODE_CANCEL = 0X0C,
    EMV_RESULT_CODE_OTHER_ERROR = 0X0F,
    EMV_RESULT_CODE_TIME_OUT = 0X0D,
    EMV_RESULT_CODE_OFFLINE_APPROVED = 0X10,
    EMV_RESULT_CODE_OFFLINE_DECLINED = 0X11,
    EMV_RESULT_CODE_REFERRAL_PROCESSING = 0X12,
    EMV_RESULT_CODE_ERROR_APP_PROCESSING = 0X13,
    EMV_RESULT_CODE_ERROR_APP_READING = 0X14,
    EMV_RESULT_CODE_ERROR_DATA_AUTH = 0X15,
    EMV_RESULT_CODE_ERROR_PROCESSING_RESTRICTIONS = 0X16,
    EMV_RESULT_CODE_ERROR_CVM_PROCESSING = 0X17,
    EMV_RESULT_CODE_ERROR_RISK_MGMT = 0X18,
    EMV_RESULT_CODE_ERROR_TERM_ACTION_ANALYSIS = 0X19,
    EMV_RESULT_CODE_ERROR_CARD_ACTION_ANALYSIS = 0X1A,
    EMV_RESULT_CODE_ERROR_APP_SELECTION_TIMEOUT = 0X1B,
    EMV_RESULT_CODE_ERROR_NO_CARD_INSERTED = 0X1C,
    EMV_RESULT_CODE_ERROR_APP_SELECTING = 0X1D,
    EMV_RESULT_CODE_ERROR_READING_CARD_APP = 0X1E,
    EMV_RESULT_CODE_ERROR_POWER_CARD_ERROR = 0X1F,
    EMV_RESULT_CODE_ERROR_NO_RESULT_CODE_PROVIDED_FOR_COMPLETION = 0X20,
    EMV_RESULT_CODE_APPROVED_WITH_ADVISE_NO_REASON = 0X21,
    EMV_RESULT_CODE_APPROVED_WITH_ADVISE_IA_FAILED = 0X22,
    EMV_RESULT_CODE_ERROR_AMOUNT_NOT_SPECIFIED = 0X23,
    EMV_RESULT_CODE_ERROR_CARD_COMPLETION = 0X24,
    EMV_RESULT_CODE_ERROR_DATA_LEN_INCORRECT = 0X25,
    EMV_RESULT_CODE_CALL_YOUR_BANK = 0X26,
    EMV_RESULT_CODE_NO_ICC_ON_CARD = 0X27,
    EMV_RESULT_CODE_NEW_SELECTION = 0X28,
    EMV_RESULT_CODE_START_TRANSACTION_SUCCESS = 0X29
} EMV_RESULT_CODE_Types;


typedef enum{
    EMV_RESULT_CODE_V2_NO_RESPONSE = -1,
    EMV_RESULT_CODE_V2_APPROVED_OFFLINE = 0x0000,
    EMV_RESULT_CODE_V2_DECLINED_OFFLINE = 0x0001,
    EMV_RESULT_CODE_V2_APPROVED = 0x0002,
    EMV_RESULT_CODE_V2_DECLINED = 0x0003,
    EMV_RESULT_CODE_V2_GO_ONLINE = 0x0004,
    EMV_RESULT_CODE_V2_CALL_YOUR_BANK = 0x0005,
    EMV_RESULT_CODE_V2_NOT_ACCEPTED = 0x0006,
    EMV_RESULT_CODE_V2_USE_MAGSTRIPE = 0x0007,
    EMV_RESULT_CODE_V2_TIME_OUT = 0x0008,
    EMV_RESULT_CODE_V2_START_TRANS_SUCCESS = 0x0010,
    EMV_RESULT_CODE_V2_MSR_SUCCESS = 0x0011,
    EMV_RESULT_CODE_V2_FILE_ARG_INVALID = 0x1001,
    EMV_RESULT_CODE_V2_FILE_OPEN_FAILED = 0x1002,
    EMV_RESULT_CODE_V2_FILE_OPERATION_FAILED = 0X1003,
    EMV_RESULT_CODE_V2_MEMORY_NOT_ENOUGH = 0x2001,
    EMV_RESULT_CODE_V2_SMARTCARD_FAIL = 0x3001,
    EMV_RESULT_CODE_V2_SMARTCARD_INIT_FAILED = 0x3003,
    EMV_RESULT_CODE_V2_FALLBACK_SITUATION = 0x3004,
    EMV_RESULT_CODE_V2_SMARTCARD_ABSENT = 0x3005,
    EMV_RESULT_CODE_V2_SMARTCARD_TIMEOUT = 0x3006,
    EMV_RESULT_CODE_V2_MSR_CARD_ERROR = 0x3007,
    EMV_RESULT_CODE_V2_PARSING_TAGS_FAILED= 0X5001,
    EMV_RESULT_CODE_V2_CARD_DATA_ELEMENT_DUPLICATE = 0X5002,
    EMV_RESULT_CODE_V2_DATA_FORMAT_INCORRECT = 0X5003,
    EMV_RESULT_CODE_V2_APP_NO_TERM = 0X5004,
    EMV_RESULT_CODE_V2_APP_NO_MATCHING = 0X5005,
    EMV_RESULT_CODE_V2_AMANDATORY_OBJECT_MISSING = 0X5006,
    EMV_RESULT_CODE_V2_APP_SELECTION_RETRY = 0X5007,
    EMV_RESULT_CODE_V2_AMOUNT_ERROR_GET = 0X5008,
    EMV_RESULT_CODE_V2_CARD_REJECTED = 0X5009,
    EMV_RESULT_CODE_V2_AIP_NOT_RECEIVED = 0X5010,
    EMV_RESULT_CODE_V2_AFL_NOT_RECEIVEDE = 0X5011,
    EMV_RESULT_CODE_V2_AFL_LEN_OUT_OF_RANGE = 0X5012,
    EMV_RESULT_CODE_V2_SFI_OUT_OF_RANGE = 0X5013,
    EMV_RESULT_CODE_V2_AFL_INCORRECT = 0X5014,
    EMV_RESULT_CODE_V2_EXP_DATE_INCORRECT = 0X5015,
    EMV_RESULT_CODE_V2_EFF_DATE_INCORRECT = 0X5016,
    EMV_RESULT_CODE_V2_ISS_COD_TBL_OUT_OF_RANGE = 0X5017,
    EMV_RESULT_CODE_V2_CRYPTOGRAM_TYPE_INCORRECT = 0X5018,
    EMV_RESULT_CODE_V2_PSE_BY_CARD_NOT_SUPPORTED = 0X5019,
    EMV_RESULT_CODE_V2_USER_LANGUAGE_SELECTED = 0X5020,
    EMV_RESULT_CODE_V2_SERVICE_NOT_ALLOWED = 0X5021,
    EMV_RESULT_CODE_V2_NO_TAG_FOUND = 0X5022,
    EMV_RESULT_CODE_V2_CARD_BLOCKED = 0X5023,
    EMV_RESULT_CODE_V2_LEN_INCORRECT = 0X5024,
    EMV_RESULT_CODE_V2_CARD_COM_ERROR = 0X5025,
    EMV_RESULT_CODE_V2_TSC_NOT_INCREASED = 0X5026,
    EMV_RESULT_CODE_V2_HASH_INCORRECT = 0X5027,
    EMV_RESULT_CODE_V2_ARC_NOT_PRESENCED = 0X5028,
    EMV_RESULT_CODE_V2_ARC_INVALID = 0X5029,
    EMV_RESULT_CODE_V2_COMM_NO_ONLINE = 0X5030,
    EMV_RESULT_CODE_V2_TRAN_TYPE_INCORRECT = 0X5031,
    EMV_RESULT_CODE_V2_APP_NO_SUPPORT = 0X5032,
    EMV_RESULT_CODE_V2_APP_NOT_SELECT = 0X5033,
    EMV_RESULT_CODE_V2_LANG_NOT_SELECT = 0X5034,
    EMV_RESULT_CODE_V2_TERM_DATA_NOT_PRESENCED = 0X5035,
    EMV_RESULT_CODE_V2_CVM_TYPE_UNKNOWN = 0X6001,
    EMV_RESULT_CODE_V2_CVM_AIP_NOT_SUPPORTED = 0X6002,
    EMV_RESULT_CODE_V2_CVM_TAG_8E_MISSING = 0X6003,
    EMV_RESULT_CODE_V2_CVM_TAG_8E_FORMAT_ERROR = 0X6004,
    EMV_RESULT_CODE_V2_CVM_CODE_IS_NOT_SUPPORTED = 0X6005,
    EMV_RESULT_CODE_V2_CVM_COND_CODE_IS_NOT_SUPPORTED = 0X6006,
    EMV_RESULT_CODE_V2_CVM_NO_MORE = 0X6007,
    EMV_RESULT_CODE_V2_PIN_BYPASSED_BEFORE = 0X6008
} EMV_RESULT_CODE_V2_Types;




typedef enum{
    EMV_AUTHORIZATION_RESULT_ACCEPTED = 0X00,
    EMV_AUTHORIZATION_RESULT_UNABLE_TO_GO_ONLINE = 0X01,
    EMV_AUTHORIZATION_RESULT_TECHNICAL_ISSUE = 0X02,
    EMV_AUTHORIZATION_RESULT_DECLINED = 0X03,
    EMV_AUTHORIZATION_RESULT_ISSUER_REFERAL = 0X04
} EMV_AUTHORIZATION_RESULT;
