//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the header file for the functions of the state machine
//           of the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __STATE_FUNCTIONS_H__
#define	__STATE_FUNCTIONS_H__


                                         
#include "Setup.h"
#include "Interrupts.h"
#include "CONSTANTS.h"


//==============================================================================
// Type definitions
//==============================================================================
typedef struct sSwitchesToRead
  {
    UINT16 ports [14];
    UINT16 bits  [14];
  } sSwitchesToRead_t;
  
typedef struct
{
    UINT32 numDigits;
    UINT32 _7seg_displays[4];
} LedDisplay;
// Global LED displays
LedDisplay display1, display2, display3, display4;


//==============================================================================
// State Machine private functions prototypes
//==============================================================================
void MAX_Write(UINT16 reg, unsigned char data);
void MAX_init();

void ProcessSW(void);
UINT8 CheckSW(void);

void Refresh_led_array(void);

// Function we call to display the data
void Display_data(void);

// Helper function to display ints and floats onto the LEDs
void Display_print_float(LedDisplay* display, float value);
void Display_print_float_fixed(LedDisplay* display, float value, int decimal_position);
void Display_print_dec(LedDisplay* display, int value);
void Display_print_digits(LedDisplay* display, char digits[4], int decimal_location);



//==============================================================================
// Macro definitions
//==============================================================================
#define ABS(x)  ( (x >= 0)?  x : -x )
#define SIGN(x) ( (x >= 0)? 1 : 2 )


//==============================================================================
// Variable declarations
//==============================================================================
// CAN variables.
// From Interrupt.c
extern volatile float CAN_Voltage;
extern volatile float CAN_Current;       
extern volatile UINT CAN_Gear;
extern volatile float CAN_Pitch;
extern volatile UINT CAN_Mast_Mode;
extern volatile UINT CAN_Pitch_Mode;
extern volatile float CAN_Mast_Direction;
extern volatile float CAN_Turb_RPM;
extern volatile float CAN_Wind_Speed;
extern volatile float CAN_Wheel_RPM;
extern volatile float CAN_Wind_Direction;
extern volatile float CAN_Loadcell;
extern volatile float CAN_Torque;

// Variables for process ROT
volatile UINT16 man_auto_amp;
volatile float Rot_deg /* = 1*/; //ernesto
float Rot_deg_ancien /*= 4000*/;

// Variables for Pushbutton detection
volatile UINT16 MemoSW[14]  /*={0}*/;
volatile UINT16 TabSW/*=0*/;
UINT16 Previous_TabSW/*=0*/;

volatile INT8 Disp1 /* = 0*/;
volatile INT8 Disp2 /*= 0*/;
volatile INT8 Disp3 /*= 0*/;


static const UINT8 i_to_7seg[10] = 
{
    NUM0_7SEG,
    NUM1_7SEG,
    NUM2_7SEG,
    NUM3_7SEG,
    NUM4_7SEG,
    NUM5_7SEG,
    NUM6_7SEG,
    NUM7_7SEG,
    NUM8_7SEG,
    NUM9_7SEG
};
 
#endif	/* __STATE_FUNCTIONS_H__ */

