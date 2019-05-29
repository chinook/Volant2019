//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook VII
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine_Volant.h
// Author  : RGB
// Date    : 2015-04-10
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : Header with all condition/transition between all states
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : NA
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef STATE_MACHINE_VOLANT_H
#define	STATE_MACHINE_VOLANT_H

#include "Setup.h"
#include "Interrupts.h"

//==============================================================================
// State Machine public function prototypes
//==============================================================================
void StateSchedulerVolant(void);            // State Scheduler. Decides which state is next
void StateInitVolant(void);                 // Initialization the display
void StateStandbyVolant(void);              // Display converted content
void StateAcq(void);                        // Get data from peripherals.
void StateCalibVolant(void);                // Calib system component such as Mast, pitch, Gear, etc.
void StateSendData(void);                   // Send CAN data.

typedef enum _STATE
{
    STATE_SCHEDULER_VOLANT,
    STATE_INIT_VOLANT,
    STATE_STANDBY_VOLANT,
    STATE_ACQ,
    STATE_CALIB_VOLANT,
    STATE_SEND_DATA,
    NUM_STATES
} STATE;


//==============================================================================
// Macro definitions
//==============================================================================

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// State scheduler flags
// The state scheduler is at the end of each state and
// decides which state is next. Following flags
// are used in this decision. The names used can and
// should be renamed to improve readability. Also, the
// conditions tested in the defines should be changed
// to proper tests
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*********** VOLANT STATE MACHINE PARAMETERS **/
/*********** BASIC CONDITION *****************/

/******* TRANSITION CONDITION INIT **********/
#define VOLANT_INIT_2_STANDBY           1

/******* TRANSITION CONDITION STANDBY ********/
#define VOLANT_STANDBY_2_INIT           0
#define VOLANT_STANDBY_2_CALIB          (Flag_Calib && Calib_Button)
//#define VOLANT_STANDBY_2_CALIB          (Flag_Calib && !Flag_InCalib)
#define VOLANT_STANDBY_2_ACQ            Flag_1ms

/******* TRANSITION CONDITION CALIB *********/
#define VOLANT_CALIB_2_INIT             0
#define VOLANT_CALIB_2_STANDBY          (Flag_Calib && !(Calib_Button))
//#define VOLANT_CALIB_2_STANDBY          (Flag_Calib && Flag_InCalib)
#define VOLANT_CALIB_2_ACQ              Flag_1ms
#define VOLANT_CALIB_2_SEND_DATA        0 // Calib_Button

/******* TRANSITION CONDITION Acq *********/
#define VOLANT_ACQ_2_SEND_DATA          GET_CAN_BUFFER_I > 0 //(oRotaryFlag || SWChange)
#define VOLANT_ACQ_2_STANDBY            !Flag_InCalib
#define VOALNT_ACQ_2_CALIB              Flag_InCalib

/******* TRANSITION CONDITION SendData *********/
#define SEND_2_STANDBY                  !Flag_InCalib
#define SEND_2_CALIB                    Flag_InCalib

//==============================================================================
// Variable declarations
//==============================================================================

// Declaration of our FSM state
extern STATE state_volant;

volatile INT8 breakFlag,       // Flag indicating if the emergency break has been pressed
              errorFlag;       // Flag indicating an error

// Variables relative to switches
extern volatile UINT16 MemoSW[14];
extern volatile UINT16 TabSW;


// Misc Flags.
volatile INT8 Calib_Button /*= -1*/;
volatile UINT8 Calib_Mast /*= 0*/;
volatile BOOL Flag_InCalib/* = 0*/;
 
// Interrupt flags
extern volatile INT8 Flag_CAN;
extern volatile INT32 Flag_50ms;
extern volatile INT32 Flag_1ms;
extern volatile INT8 Flag_Calib;
extern volatile UINT oRotary_Flag;
extern volatile UINT CAN_Acq_Stat;
 
// CAN message variables
extern volatile UINT CAN_Mast_Mode;
extern volatile UINT CAN_Pitch_Mode;
extern volatile UINT  CAN_Calib_Done;

 
// For Calib LED flash. When Calib done.
volatile UINT8 iLEDFlash /*= 2*/;

volatile UINT8 SWChange /*= 0*/;

// Debug variable
volatile UINT16 error /* = 0*/;
 
#endif	/* STATE_MACHINE_VOLANT_H */

