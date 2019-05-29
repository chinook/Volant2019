//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook VII
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateMachine_Volant.c
// Author  : Gabriel Jean-Louis
// Date    : 2017-06-23
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : 
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : NA
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "StateMachine_Volant.h"
#include "StateFunctions.h"
#include "Interrupts.h"
#include "ChinookSteeringWheelUtils.h"

// Definition of our state
STATE state_volant;
 
//==============================================================================
//	STATES OF STATE MACHINE
//==============================================================================

//===============================================================
// Name     : StateScheduler_Volant
// Purpose  : Decides which state is next depending on current
//            state and flags. Used as a function
//===============================================================
void StateSchedulerVolant(void)
{
    // This is will select our next state based on flags
    switch(state_volant)
    {
        case STATE_INIT_VOLANT:
            //if (VOLANT_INIT_2_STANDBY)
            //{
                state_volant = STATE_STANDBY_VOLANT;
            //}
            //else
            //{
            //    state_volant = STATE_INIT_VOLANT;
            //}
            break;
            
        case STATE_STANDBY_VOLANT:
            if (VOLANT_STANDBY_2_ACQ)
            {
                state_volant = STATE_ACQ;
            }  
            else if (VOLANT_STANDBY_2_CALIB)
            {
                state_volant = STATE_CALIB_VOLANT;
            }
            else
            {
                state_volant = STATE_STANDBY_VOLANT;
            }
            break;
            
        case STATE_ACQ:
            if (VOLANT_ACQ_2_SEND_DATA)
            {
                state_volant = STATE_SEND_DATA;
            }
            else if (VOALNT_ACQ_2_CALIB)
            {
                state_volant = STATE_CALIB_VOLANT;
            }
            else if (VOLANT_ACQ_2_STANDBY)
            {
                state_volant = STATE_STANDBY_VOLANT;
            }
            else
            {
                state_volant = STATE_ACQ;
            }
            break;
            
        case STATE_CALIB_VOLANT:
            if (VOLANT_CALIB_2_ACQ)
            {
                state_volant = STATE_ACQ;
            }
            else if (VOLANT_CALIB_2_STANDBY)
            {
                state_volant = STATE_STANDBY_VOLANT;
            }
            else if(VOLANT_CALIB_2_SEND_DATA)
            {
                state_volant = STATE_SEND_DATA;
            }    
            else
            {
                state_volant = STATE_CALIB_VOLANT;
            }
            break;
            
        case STATE_SEND_DATA:
            if (SEND_2_STANDBY)
            {
                state_volant = STATE_STANDBY_VOLANT;
            }
            else if (SEND_2_CALIB)
            {
                state_volant = STATE_CALIB_VOLANT;
            }
            else
            {
                state_volant = STATE_SEND_DATA;
            }
            break;
            
        default:
            state_volant = STATE_INIT_VOLANT;
    }
}

//==============================================================================
//	INITIALISATION STATE
//==============================================================================

//===============================================================
// Name     : StateInitVolant
// Purpose  : Initialization of the system.
//===============================================================
void StateInitVolant(void)
{
    INTDisableInterrupts();   // Disable all interrupts of the system.
    InitPorts();
    //InitWdt();
    InitTimer();
    //InitTimeCapture();
    //InitPwm();
    InitAdc();
    //InitUart();
    InitCan();
    InitSpi();
    //InitI2C();
    MAX_init();
    StartInterrupts();
    
    // Send connection to backplane 
    //Can.SendByte(CAN1, BACKPLANE_BOARD_SID , 0x42);
    Can.SendByte(CAN1, BACKPLANE_BOARD_SID , VOLANT_BOARD_SID);
}

//==============================================================================
//	STANDBY STATE
//==============================================================================
//===============================================================
// Name       : StateStandbyVolant
// Purpose    : The standby state is the loop where the system stays when no input
//              in detected, either from the switches or from the CANbus.
//              A check is made using CheckSW() to monitor the switches activity.
//              The LEDs on the dash are also refreshed with the lastest values
//              at a 20Hz refresh rate.
//===============================================================

void StateStandbyVolant(void)
{
    if (CAN_Acq_Stat == ACQ_ON)
    {
        //led_array.daq_on = 1;     //This LED is broken on the current volant
        led_array.mast_mode = 1;    // Acq LED is replaced by this one
    }
    else
    {
        //led_array.daq_on = 0;
        led_array.mast_mode = 0;
    }
    
    // <editor-fold defaultstate="collapsed" desc="Switch pressed or released?"> 
    if (SWChange)
    {
        SWChange = 0;
        
        // Will trigger the jump to Calib state if the Calib engage button is pressed for 3s.
        if ((TabSW & CALIB_ENGAGE_BUTTON) == CALIB_ENGAGE_BUTTON)
        {
            // Still waiting for the 3s to elapse.
            if (Calib_Button == -1)
            {
                Calib_Button = 1;
                error = Timer.EnableInterrupt(TIMER_1);
                Flag_Calib = 0;     // Reset interrupt flag.
            }               
        }
        else
        {
            // Button was released.
            if (Calib_Button == 1)
            {
                Timer.DisableInterrupt(TIMER_1);
                Calib_Button = -1;
            }
        }
    }
    // </editor-fold>

    if (Flag_50ms)        // Refresh at 20 Hz
    {
        Flag_50ms = 0;
        Display_data();
    }
}


//===============================================================
// Name     : StateCalibVolant
// Purpose  : Calibrate system. Zero Mast, Pitch. Adjust Gear and Weather system.
// Global   : 
//===============================================================
void StateCalibVolant(void)
{
    // <editor-fold defaultstate="collapsed" desc="Reset Flags">
    if (Flag_InCalib == 0)
    {
        // Indicates that we are in calib state.
        Flag_InCalib = 1;
        Calib_Button = -1;
        Timer.DisableInterrupt(TIMER_1);
        Flag_Calib = 0;             // Reset interrupt flag.
//        iCurCalib = GEAR_CALIB;     // Reset the CurCalib to GEAR_CALIB the first calib possible.
    }
    // </editor-fold>
    
    // Lights LEDs according to the system parameters.
    if(CAN_Mast_Mode == MAST_AUTO_MODE)
    {
        led_array.mast_mode = 1;
    }
    else
    {
        led_array.mast_mode = 0;
    }
        
    // <editor-fold defaultstate="collapsed" desc="Switch pressed or released?">                          
    if (SWChange)
    {
        SWChange = 0;
       
        // Toggle button pressed?
        if ((TabSW & CALIB_TOGGLE_BUTTON) == CALIB_TOGGLE_BUTTON)
        {
//            iCurCalib++;
     //       if (iCurCalib == MAX_CALIB)
     //       {
     //           iCurCalib = GEAR_CALIB;
        //    }                                         
        }

        // Will trigger the jump to Standby state if the Calib engage button is pressed for 3s.
        if ((TabSW & CALIB_ENGAGE_BUTTON) == CALIB_ENGAGE_BUTTON)
        {
            // Still waiting for the 3s to elapse.
            if (Calib_Button == -1)
            {
                Calib_Button = 0;
                error = Timer.EnableInterrupt(TIMER_1);
                Flag_Calib = 0;     // Reset interrupt flag.                
            }               
        }
        else
        {
            // Button was released.
            if (Calib_Button == 0)
            {
                Timer.DisableInterrupt(TIMER_1);
                // Indicates that the button was pressed momentarily.
                // Send set zero.
                Calib_Button = 1;
                
                Flag_Calib = 0;     // Reset interrupt flag.
            }
        }
    }
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Set zero logic for each system"> 
    
    /*
    switch (iCurCalib)
    {
        case GEAR_CALIB:
            led_array.mast_calib = 0;
            led_array.gear_calib = 1;
            
            // Send set zero to Gear Board.
            if (Calib_Button)
            {                
                Calib_Button = -1;
                SID_CAN_BUFFER = VOLANT_GEAR_ZERO;
                MSG_CAN_BUFFER = EMPTY_CAN_MSG;
                CAN_BUFFER_INCREASE;
            }
            
            if (oCalibDone)
            {
                for (iLEDFlash = 0; iLEDFlash < 3; iLEDFlash++)
                {
                    Timer.DelayMs(300);
                    led_array.gear_calib = 0;
                    Timer.DelayMs(300);
                    led_array.gear_calib = 1;
                }  
//                CAN_MastCalib=0;
                oCalibDone = 0;
            }
            break;
            
        case PITCH_CALIB:
            led_array.gear_calib = 0;
            led_array.pitch_calib = 1;
            
            // Send set zero to Pitch Board.
            if (Calib_Button)
            {               
                Calib_Button = -1;
                SID_CAN_BUFFER = VOALNT_PITCH_ZERO;
                MSG_CAN_BUFFER = EMPTY_CAN_MSG;
                CAN_BUFFER_INCREASE;
            }
            
            if (oCalibDone)
            {
                for (iLEDFlash = 0; iLEDFlash < 3; iLEDFlash++)
                {
                    Timer.DelayMs(300);
                    led_array.pitch_calib = 0;
                    Timer.DelayMs(300);
                    led_array.pitch_calib = 1;
                }  
//                CAN_MastCalib=0;
                oCalibDone = 0;
            }
            
            break;
            
        case WEATHERCOCK_CALIB:
            led_array.pitch_calib = 0;
            led_array.weather_calib = 1;
            
            // Send set zero to SensTel Board.
            if (Calib_Button)
            {                
                Calib_Button = -1;
                SID_CAN_BUFFER = VOLANT_WEATHER_ZERO;
                MSG_CAN_BUFFER = EMPTY_CAN_MSG;
                CAN_BUFFER_INCREASE;
            }
            
            if (oCalibDone)
            {
                for (iLEDFlash = 0; iLEDFlash < 3; iLEDFlash++)
                {
                    Timer.DelayMs(300);
                    led_array.weather_calib = 0;
                    Timer.DelayMs(300);
                    led_array.weather_calib = 1;
                }  
//                CAN_MastCalib=0;
                oCalibDone = 0;
            }
            
            break;
            
        case MAST_CALIB:
            led_array.weather_calib = 0;
            led_array.mast_calib = 1;     
            
            // Send set zero to Mast Board.
            if (Calib_Button)
            {                
                Calib_Button = -1;
                SID_CAN_BUFFER = VOLANT_MAST_ZERO;
                MSG_CAN_BUFFER = EMPTY_CAN_MSG;
                CAN_BUFFER_INCREASE;
            }
            
            // If we received the command from the Mast board that confirms mast set to zero.
            // MAST Calib LED will flash 3 times to indicate that the Mast has been calibrated.
//            if (CAN_MastCalib)
            if (oCalibDone)
            {
                for (iLEDFlash = 0; iLEDFlash < 3; iLEDFlash++)
                {
                    Timer.DelayMs(300);
                    led_array.mast_calib = 0;
                    Timer.DelayMs(300);
                    led_array.mast_calib = 1;
                }  
//                CAN_MastCalib=0;
                oCalibDone = 0;
            }
            break;    
    }
    // </editor-fold>
    */
    // Refresh displays. At 20Hz.
    if (Flag_50ms)
    {
        Flag_50ms = 0;
        
        Display_data();
    }
}

//===============================================================
// Name     : StateAcq
// Purpose  : Get data from ADC and Switches.
// Global   : No return value.
//===============================================================
void StateAcq(void)
{
    // Flags for doing average.
    static UINT16   man_auto_count = 0;
    static UINT16   man_auto_add = 0;
    
    Flag_1ms = 0;
    
    if (oRotary_Flag)
    {
      man_auto_count++;
      man_auto_add = man_auto_add + Adc.Var.adcReadValues[2];

      if (man_auto_count==10)
      {
          man_auto_amp  = man_auto_add/10;      
          man_auto_count = 0;
          man_auto_add = 0;
      }
    }

    SWChange = CheckSW();

    if (SWChange)
    {
        ProcessSW();
    }
}

//===============================================================
// Name     : StateSendData
// Purpose  : This state will send all the data in the CAN buffer.
//===============================================================
void StateSendData(void)
{   
    while (GET_CAN_BUFFER_I > 0)
    {
        CAN_BUFFER_DECREASE;
        SEND_CAN_DATA;        
        
    }
    
    GET_CAN_BUFFER_I = 0;
    
    //Can.SendData(CAN1, 66, 1);
}