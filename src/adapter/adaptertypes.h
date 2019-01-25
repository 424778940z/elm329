/**
 * See the file LICENSE for redistribution information.
 *
 * Copyright (c) 2009-2016 ObdDiag.Net. All rights reserved.
 *
 */

#ifndef __ADAPTER_TYPES_H__ 
#define __ADAPTER_TYPES_H__

#include <cstdint>
#include <cstring>
#include <lstring.h>
#include <adapterdefs.h>

using namespace std;

// Config settings
const int KWP_HDR_LEN      = 5; // 4 header + 1 chksum
const int OBD_IN_MSG_DLEN  = 255;                            // Binary len
const int OBD_OUT_MSG_DLEN = 255;                            // Binary len
const int OBD_OUT_MSG_LEN  = OBD_OUT_MSG_DLEN + KWP_HDR_LEN; // Binary buffer size
const int TX_BUFFER_LEN    = OBD_OUT_MSG_LEN * 3;            // Char buffer size

//
// Command dispatch values
//
const int BYTE_PROPS_START  = 0;
const int INT_PROPS_START   = 100;
const int BYTES_PROPS_START = 1000;

enum AT_Requests {
    // bool properties
    PAR_ADAPTER_SIGNATURE = BYTE_PROPS_START,
    PAR_ADPTV_TIM0,
    PAR_ADPTV_TIM1,
    PAR_ADPTV_TIM2,
    PAR_ALLOW_LONG,
    PAR_BUFFER_DUMP,
    PAR_BYPASS_INIT,
    PAR_CALIBRATE_VOLT,
    PAR_CAN_CAF,
    PAR_CAN_DLC,
    PAR_CAN_FLOW_CONTROL,
    PAR_CAN_MONITORING,
    PAR_CAN_SEND_RTR,
    PAR_CAN_SHOW_STATUS,
    PAR_CAN_VAIDATE_DLC,
    PAR_CHIP_COPYRIGHT,
    PAR_DESCRIBE_PROTCL_N,
    PAR_DESCRIBE_PROTOCOL,
    PAR_ECHO,
    PAR_GET_SERIAL,
    PAR_HEADER_SHOW,
    PAR_INFO,
    PAR_KW_CHECK,
    PAR_KW_DISPLAY,
    PAR_LINEFEED,
    PAR_LOW_POWER_MODE,
    PAR_MEMORY,
    PAR_PROTOCOL_CLOSE,
    PAR_READ_VOLT,
    PAR_RESET_CPU,
    PAR_RESPONSES,
    PAR_SERIAL,
    PAR_SET_DEFAULT,
    PAR_SPACES,
    PAR_STD_SEARCH_H,
    PAR_TRY_PROTOCOL,
    PAR_USE_AUTO_SP,
    PAR_VERSION,
    PAR_WARMSTART,
    PAR_WIRING_TEST,
    PAR_DUMMY,
    BYTE_PROPS_END,
    // int properties
    PAR_CAN_FLOW_CTRL_MD = INT_PROPS_START,
    PAR_CAN_SET_ADDRESS,
    PAR_CAN_TIMEOUT_MULT,
    PAR_CAN_TSTR_ADDRESS,
    PAR_ISO_INIT_ADDRESS,
    PAR_PROTOCOL,
    PAR_SET_BRD,
    PAR_TESTER_ADDRESS,
    PAR_TIMEOUT,
    PAR_TRY_BRD,
    PAR_WAKEUP_VAL,
    INT_PROPS_END,
    // bytes properties
    PAR_CAN_EXT = BYTES_PROPS_START,
    PAR_CAN_FILTER,
    PAR_CAN_FLOW_CTRL_DAT,
    PAR_CAN_FLOW_CTRL_HDR,
    PAR_CAN_MASK,
    PAR_CAN_PRIORITY_BITS,
    PAR_HEADER_BYTES,
    PAR_USER_B,
    PAR_WM_HEADER,
    BYTES_PROPS_END
};

//
// union IntAggregate
//
union IntAggregate
{
    uint32_t lvalue;  
    uint8_t  bvalue[4];  
    IntAggregate() { lvalue = 0; }
    IntAggregate(uint32_t val) { lvalue = val; }
    IntAggregate(uint8_t b0, uint8_t b1, uint8_t b2 = 0, uint8_t b3 = 0) {
        bvalue[0] = b0; 
        bvalue[1] = b1; 
        bvalue[2] = b2; 
        bvalue[3] = b3; 
    }
};

struct ByteArray {
    const static int ARRAY_SIZE = 7;
    ByteArray() {
        clear();
    }
    uint32_t asCanId() const {
        if (length == 4) {
            IntAggregate val(data[3], data[2], data[1], data[0]);
            return val.lvalue;
        }
        else if (length == 2) {
            IntAggregate val(data[1], data[0]);
            return val.lvalue;
        }
        return 0;
    }
    void clear() {
    	length = 0;
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
    void setBoolProperty(int parameter, bool val);
    bool getBoolProperty(int parameter) const;
    void setIntProperty(int parameter,  uint32_t val);
    uint32_t getIntProperty(int parameter) const;
    void setBytesProperty(int parameter, const ByteArray* bytes);
    const ByteArray* getBytesProperty(int parameter) const;
    void clear();
private:
    const static int BYTE_PROP_LEN  = BYTE_PROPS_END - BYTE_PROPS_START;
    const static int INT_PROP_LEN   = INT_PROPS_END - INT_PROPS_START;
    const static int BYTES_PROP_LEN = BYTES_PROPS_END - BYTES_PROPS_START;

    AdapterConfig();
    uint64_t   values_;
    uint8_t    byteProps_ [BYTE_PROP_LEN];
    uint32_t   intProps_  [INT_PROP_LEN];
    ByteArray  bytesProps_[BYTES_PROP_LEN];
};

class DataCollector;
void AdptSendString(const util::string& str);
void AdptSendReply(const char* str);
void AdptSendReply(const util::string& str);
void AdptSendReply(util::string& str);
void AdptDispatcherInit();
void AdptOnCmd(const DataCollector* collectorg);
void AdptReadSerialNum();
void AdptPowerModeConfigure();

// Utilities
void Delay1ms(uint32_t value);
void StartDelay1ms(uint32_t value);
void Delay1us(uint32_t value);
void KWordsToString(const uint8_t* kw, util::string& str);
void CanIDToString(uint32_t num, util::string& str, bool extended);
void AutoReceiveParse(const util::string& str, uint32_t& filter, uint32_t& mask);

uint32_t to_bytes(const util::string& str, uint8_t* bytes);
void to_ascii(const uint8_t* bytes, uint32_t length, util::string& str);

// LEDs
#define TX_LED(val) GPIOPinWrite(TX_LED_PORT, TX_LED_NUM, val)
#define RX_LED(val) GPIOPinWrite(RX_LED_PORT, RX_LED_NUM, val)

#endif //__ADAPTER_TYPES_H__
