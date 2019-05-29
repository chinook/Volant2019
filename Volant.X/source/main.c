
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Volant5
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : main.c
// Author  : Romain Guilbault (RGB)
// Date    : 2015-03-29
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the code used to drive the wheel PCB including the 7seg
//           and push buttons
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Setup.h"
#include "Interrupts.h"
#include "StateMachine_Volant.h"
#include "HardwareProfile.h"
#include "StateFunctions.h"


//==============================================================================
// MAIN CODE
//==============================================================================
void main(void)
{

//==============================================================================
// Following memcopies are used to be able to use the form 
// Chinook.LibraryX.FunctionX in case the developper wants to see the available
// functions.
//==============================================================================
  memcpy( &Chinook.Port  , &Port  , sizeof ( struct sChinookPort  ) );
  memcpy( &Chinook.Uart  , &Uart  , sizeof ( struct sChinookUart  ) );
  memcpy( &Chinook.Pwm   , &Pwm   , sizeof ( struct sChinookPwm   ) );
  
  //up
  //memcpy( &Chinook.Timer , &Timer , sizeof ( struct sChinookTimer ) );
  
  
  memcpy( &Chinook.Spi   , &Port  , sizeof ( struct sChinookSpi   ) );
  
 // up
  memcpy( &Chinook.Wdt   , &Wdt   , sizeof ( struct sChinookWdt   ) );
  memcpy( &Chinook.Adc   , &Adc   , sizeof ( struct sChinookAdc   ) );
  memcpy( &Chinook.Can   , &Can   , sizeof ( struct sChinookCan   ) );
  
 // up
//  memcpy( &Chinook.I2C   , &I2C   , sizeof ( struct sChinookI2C   ) );
//  memcpy( &Chinook.Skadi , &I2C   , sizeof ( struct sChinookSkadi ) );
//  memcpy( &Chinook.InputCapture , &InputCapture , sizeof ( struct sChinookInputCapture ) );
//==============================================================================


//==============================================================================
// The next line disables the JTAG for the PIC. If the JTAG is enabled, pins
// RB10-13 can't be used as I/Os. If you want to use the JTAG, comment or
// remove this line.
//==============================================================================
  DDPCONbits.JTAGEN = 0;

//==============================================================================


//==============================================================================
// Configure the device for maximum performance but do not change the PBDIV
// Given the options, this function will change the flash wait states, RAM
// wait state and enable prefetch cache but will not change the PBDIV.
// The PBDIV value is already set via the pragma FPBDIV option in HardwareProfile.h.
//==============================================================================
  SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
//==============================================================================

// State machine init
//============================
	state_volant = STATE_INIT_VOLANT;
//============================

	while(1)  //infinite loop
	{
        // jump to next state
        switch(state_volant)
        {
            case STATE_INIT_VOLANT:
                StateInitVolant();
                break;
            case STATE_STANDBY_VOLANT:
                StateStandbyVolant();
                break;
            case STATE_ACQ:
                StateAcq();
                break;
            case STATE_CALIB_VOLANT:
                StateCalibVolant();
                break;
            case STATE_SEND_DATA:
                StateSendData();
                break;
            default:
                StateInitVolant();
        }
        
        // Decides which state will be next
        StateSchedulerVolant();
        
        LED_STATUS= 0;
	}
} //END MAIN CODE