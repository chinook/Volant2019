//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook VII
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : CONSTANTS.h
// Author  : Gabriel Jean-Louis
// Date    : 2017-06-22
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : Header with all the constants needed.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : NA
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "Setup.h"

#ifndef _CONSTANTS_H    /* Guard against multiple inclusion */
#define _CONSTANTS_H

typedef struct{
    UINT8    sID[CAN_BUFFER_SIZE];
    INT16    message[CAN_BUFFER_SIZE];
    INT8     i;
}CAN_buffer_t;

CAN_buffer_t    can_buffer /*= {{0}, {0}, 0}*/;

// bitfield for LED array.
typedef struct{
    // First 8 LED array bit field.
    union{
        UINT8   F8;
        struct{
            UINT8    rmp_500         : 1;
            UINT8    pitch_mode      : 1;
            UINT8    mast_calib      : 1;
            UINT8    pitch_calib     : 1;
            UINT8    gear_calib      : 1;
            UINT8    mast_mode       : 1;
            UINT8    daq_on          : 1;
            UINT8    weather_calib   : 1;
        };
    };
    
    // Last 5 LED array bit field.
    union{
        UINT8 L5;
        struct{
            UINT8    unused          : 2;
            UINT8    rmp_1000        : 1;
            UINT8    rpm_900         : 1;
            UINT8    rmp_800         : 1;
            UINT8    rmp_700         : 1;
            UINT8    rmp_600         : 1;
            UINT8    unused8         : 1;
        };
    };
}led_array_t;

led_array_t     led_array;

// Register Address Map
#define REG_NO_OP               0x00
#define REG_DECODE_MODE         0x01
#define REG_GLOBAL_INTENSITY    0x02
#define REG_SCANLIMIT           0x03
#define REG_CONFIG              0x04
#define REG_DISPLAY_TEST        0x07
#define REG_DIGIT_TYPE          0x0C
#define REG_DIGIT1_7SEG         0x20
#define REG_DIGIT2_7SEG         0x28
#define REG_DIGIT3_7SEG         0x21
#define REG_DIGIT4_7SEG         0x29
#define REG_DIGIT5_7SEG         0x22
#define REG_DIGIT6_7SEG         0x2A
#define REG_DIGIT7_7SEG         0x23 
#define REG_DIGIT8_7SEG         0x2B
#define REG_DIGIT9_7SEG         0x24
#define REG_DIGIT10_7SEG        0x2C
#define REG_DIGIT11_7SEG        0x25
#define REG_DIGIT12_7SEG        0x2D
#define REG_DIGIT13_7SEG        0x2E
#define REG_DIGIT14_7SEG        0x2F
#define REG_DIGIT15_7SEG        0x2F
#define REG_DIGIT16_7SEG        0x2F


#define LED_ARRAY_FIRST8        0x25   // Defines the first 8 LEDs of the array
#define LED_ARRAY_LAST8         0x2D   // Defines the last 7 LEDs of the array

// Configuration constants
#define NO_DECODE               0x00   // Places all 12 digit register in non decode mode.
#define SCAN_LIMIT              0x05   // Defines only 12 digit registers will be scanned for refresh.
#define CONFIG                  0x11   // ShutDown mode: Normal operation and Global blink synch: Reset on rising edge of CS_n.
#define BRIGHTNESS              0x07   // Brightness set at halfway.
#define DISP_TYPE               0x00   // Digit 7 to 0 are 7segments displays.

// 7 Segment Display numbers
#define NUM0_7SEG               0x7E
#define NUM1_7SEG               0x30
#define NUM2_7SEG               0x6D
#define NUM3_7SEG               0x79
#define NUM4_7SEG               0x33
#define NUM5_7SEG               0x5B
#define NUM6_7SEG               0x5F
#define NUM7_7SEG               0x70
#define NUM8_7SEG               0x7F
#define NUM9_7SEG               0x7B



#define MINUS_7SEG              0x01
#define DP_7SEG                 0x80
#define BLANK_7SEG              0x00

// Constants for CAN Message.
#define MAST_MANUAL_MODE            0x01
#define MAST_AUTO_MODE              0x00
#define PITCH_MANUAL_MODE           0x01
#define PITCH_AUTO_MODE             0x00
#define V_BATT_MIN                  19.0
#define RPM_500                     (CAN_WindTurbRPM <= 500)
#define RPM_600                     (CAN_WindTurbRPM >= 500)
#define RPM_700                     (CAN_WindTurbRPM >= 500)
#define RPM_800                     (CAN_WindTurbRPM >= 500)
#define RPM_900                     (CAN_WindTurbRPM >= 500)
#define RPM_1000                    (CAN_WindTurbRPM >= 500)

// CAN buffer Macros
#define SID_CAN_BUFFER             can_buffer.sID[can_buffer.i]
#define MSG_CAN_BUFFER             can_buffer.message[can_buffer.i]
#define CAN_BUFFER_INCREASE        can_buffer.i++
#define CAN_BUFFER_DECREASE        can_buffer.i--
#define GET_CAN_BUFFER_I           can_buffer.i
#define SEND_CAN_DATA              Can.SendData(CAN1, can_buffer.sID[can_buffer.i], can_buffer.message[can_buffer.i])

#define ACQ_ON                     1

// Display navigation
enum SPEED_DISP
{
    WHEEL_SPEED = 0,
    WIND_DIR,
    WIND_SPEED,
    //MAST_COMMAND,
    //AMP_ROT,
    CURRENT,
    
    // Keep last
    MAX_VALUE_SPEED_DISP    
};

enum RPM_DISP
{
    TURB_RPM = 0,
    PITCH_ANGLE,
    MAST_DIR,
    //WIND_DIR,
    VOLTAGE,
    // Keep last
    MAX_VALUE_RPM_DISP
};

enum DISPLAY4_ENUM
{
  EFFICACITE,
  VEHICULE_SPEED,
  TORQUE,
  LOADCELL,
  MAX_VALUE_DISPLAY4
};

#endif /* _CONSTANTS_H */

/* *****************************************************************************
 End of File
 */
