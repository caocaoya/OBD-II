/**
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2009-2016 ObdDiag.Net. All rights reserved.
 *
 */

#ifndef __ADAPTER_TYPES_H__ 
#define __ADAPTER_TYPES_H__

#include <adapter_inc/adapterdefs.h>
#include <utility_inc/lstring.h>

#include <cstdint>
#include <cstring>

using namespace std;

// Config settings
const int OBD_IN_MSG_DLEN = 7; 
const int OBD_IN_MSG_LEN  = OBD_IN_MSG_DLEN + 5; // 7 data + 4 header + 1 reserved
const int RX_BUFFER_LEN   = 100; 
const int RX_CMD_LEN      = 20;         // The incoming cmd
const int USER_BUF_LEN    = RX_CMD_LEN; // The previous cmd

//
// Command dispatch values
//
const int INT_PROPS_START   = 100;
const int BYTES_PROPS_START = 1000;

enum AT_Requests {
    // bool properties
    PAR_ALLOW_LONG = 0,
	PAR_AUTO_RECEIVE,
    PAR_BUFFER_DUMP,
	PAR_BYPASS_INIT,
    PAR_CALIBRATE_VOLT,
    PAR_CAN_CAF,
    PAR_CAN_DLC,
	PAR_CAN_MONITORING,
	PAR_STD_SEARCH_MODE,
    PAR_CHIP_COPYRIGHT,
    PAR_DESCRIBE_PROTCL_N,
    PAR_DESCRIBE_PROTOCOL,
    PAR_ECHO,
    PAR_GET_SERIAL,
    PAR_HEADER_SHOW,
    PAR_INFO,
	PAR_J1939_FMT,
	PAR_J1939_HEADER,
	PAR_J1939_MLTPR5,
    PAR_KW_CHECK,
    PAR_KW_DISPLAY,
    PAR_KWP_4BYTES,
    PAR_LINEFEED,
    PAR_MEMORY,
    PAR_PROTOCOL_CLOSE,
    PAR_READ_VOLT,
    PAR_RESET_CPU,
	PAR_RESPONSES,
    PAR_SECOND_K,
    PAR_SET_DEFAULT,
    PAR_PROTOCOL,
    PAR_SERIAL,
    PAR_SPACES,
    PAR_TRY_PROTOCOL,
    PAR_VERSION,
    PAR_WARMSTART,
    PAR_WIRING_TEST,
    PAR_USE_AUTO_SP,
	PAR_ADAPTIVE_TIMING,
	PAR_CAN_SHOW_STATUS,
	PAR_J1939_DM1_MONITOR,
	PAR_FORGET_EVENTS,
	PAR_FAST_INIT,
	PAR_ISO_BAUDRATE,
	PAR_INFRAME_RESPONSE,
	PAR_LOW_POWER_MODE,
	PAR_CAN_SEND_RTR,
	PAR_SLOW_INIT,
	PAR_CAN_VAIDATE_DLC,
	PAR_J1939_MONITOR,
    // int properties
    PAR_CAN_CF = INT_PROPS_START,
    PAR_CAN_CM,
    PAR_CAN_CP,
	PAR_CAN_EXT,
	PAR_CAN_SET_ADDRESS,
	PAR_CAN_FLOW_CONTROL,
	PAR_CAN_FLOW_CTRL_HDR,
    PAR_ISO_INIT_ADDRESS,
	PAR_RECEIVE_ADDRESS,
	PAR_RECEIVE_FILTER,
	PAR_TESTER_ADDRESS,
	PAR_TRY_BRD,
	PAR_SET_BRD,
    PAR_TIMEOUT,
    PAR_WAKEUP_VAL,
    // bytes properties
    PAR_HEADER_BYTES = BYTES_PROPS_START,
	PAR_CAN_FLOW_CTRL_DAT,
    PAR_WM_HEADER
};

struct ByteArray {
    const static int ARRAY_SIZE = 7;
    ByteArray() : length(0) { 
        memset(data, 0, sizeof(data));
    }
    uint8_t data[ARRAY_SIZE];
    uint8_t length;
};

// Configuration settings
//
class AdapterConfig {
public:
    static AdapterConfig* instance();
    void     setBoolProperty(int parameter, bool val);
    bool     getBoolProperty(int parameter) const;
    void     setIntProperty(int parameter,  uint32_t val);
    uint32_t getIntProperty(int parameter) const;
    void     setBytesProperty(int parameter, const ByteArray* bytes);
    const    ByteArray* getBytesProperty(int parameter) const;
private:
    const static int INT_PROP_LEN   = (PAR_WAKEUP_VAL - INT_PROPS_START   + 1);
    const static int BYTES_PROP_LEN = (PAR_WM_HEADER  - BYTES_PROPS_START + 1);

    AdapterConfig();
    uint64_t   values_; // 64 max
    uint32_t   intProps_  [INT_PROP_LEN];
    ByteArray  bytesProps_[BYTES_PROP_LEN];
};

union NumericType
{
    uint32_t lvalue;  
    uint8_t  bvalue[4];  
    NumericType() { lvalue = 0; }
    NumericType(uint32_t val) { lvalue = val; }
    NumericType(uint8_t b0, uint8_t b1, uint8_t b2 = 0, uint8_t b3 = 0) {
        bvalue[0] = b0; 
        bvalue[1] = b1;
        bvalue[2] = b2;
        bvalue[3] = b3; 
    }
};

void AdptSendString(const util::string& str);
void AdptSendReply(const util::string& str);
void AdptDispatcherInit();
void AdptOnCmd(util::string& cmdString);
void AdptCheckHeartBeat();
void AdptReadSerialNum();
void AdptPowerModeConfigure();

// Utilities
void Delay1ms(uint32_t value);
void Delay1us(uint32_t value);
void KWordsToString(const uint8_t* kw, util::string& str);
void CanIDToString(uint32_t num, util::string& str, bool extended)
;

uint32_t to_bytes(const util::string& str, uint8_t* bytes);
void to_ascii(const uint8_t* bytes, uint32_t length, util::string& str);

// LEDs
//#define TX_LED(val) GPIOPinWrite(TX_LED_PORT, TX_LED_NUM, (~val) & 0x1)
//#define RX_LED(val) GPIOPinWrite(RX_LED_PORT, RX_LED_NUM, (~val) & 0x1)

#endif //__ADAPTER_TYPES_H__
