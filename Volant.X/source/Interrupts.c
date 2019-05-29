//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Interrupts.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file declares all interrupt subroutines used.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : All priorities must be set and verified by the developper. All
//           priorities and subpriorities used are there as example. Also, note
//           that interrupt can be defined as iplnauto, iplnsrs and iplnsoft,
//           where n can be from 0 to 7. 7 is the highest priority, while 1 is
//           the lowest. A priority of 0 will disable the interrupt.
//           Subpriorities can go from (lowest to highest) 0 to 3.
//           SRS mode uses the shadow registers, soft uses the software
//           registers and auto is an automatic assignment.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "Interrupts.h"
#include "ChinookSteeringWheelUtils.h"

volatile INT32 Flag_500ms = 0;
volatile INT32 Flag_50ms = 0;
volatile INT32 Flag_1ms = 0;
volatile INT8  Flag_CAN = 0;
volatile INT8  Flag_Calib = 0;

// CAN Messages from other boards
volatile UINT CAN_Gear = 0;
volatile float CAN_Pitch = 1234;
volatile UINT CAN_Mast_Mode = 1; // 1 is the value from MAST board at start up
volatile UINT CAN_Pitch_Mode = 0;
volatile float CAN_Mast_Direction = 0;
volatile UINT  CAN_Calib_Done = 0;
volatile float CAN_Turb_RPM = 22;
volatile float CAN_Wind_Speed = 33;
volatile float CAN_Current = 99;
volatile float CAN_Voltage = 22.2;
volatile float CAN_Wheel_RPM = 55;
volatile float CAN_Wind_Direction = 44;
volatile UINT CAN_Acq_Stat = 0;
volatile float CAN_Loadcell = 77.0;
volatile float CAN_Torque = 66.0;

volatile UINT oRotary_Flag = 0;

/*
static void* CAN_msg_dests[] = 
{
    (void*)&CAN_Gear,
    (void*)&CAN_Pitch,
    (void*)&CAN_Mast_Direction,
    (void*)&CAN_Mast_Mode,
    (void*)&CAN_Calib_Done,
    (void*)&CAN_Turb_RPM,
    (void*)&CAN_Wind_Speed,
    (void*)&CAN_Current,
    (void*)&CAN_Voltage,
    (void*)&CAN_Wheel_RPM,
    (void*)&CAN_Wind_Direction,
    (void*)&CAN_Acq_Stat,
    (void*)&CAN_Pitch_Mode,
    (void*)&CAN_Loadcell,
    (void*)&CAN_Torque
};
*/

// Used for 3s delay. 
//Impossible to have a timer at 3s seconds due to high clock frequency.
//volatile INT8 delay = 3;

//==============================================================================
//	TIMER INTERRUPTS
//==============================================================================

//=============================================
// Configure the Timer 1 interrupt handler
//=============================================
void __ISR(_TIMER_1_VECTOR, T1_INTERRUPT_PRIORITY) Timer1InterruptHandler(void)
{
    static UINT8 i = 3; // For 3 seconds delay.
    //This creates a 3s delay.
    i--;
    //delay--;
    if (i == 0)
    {
        Flag_Calib = 1;
        i = 3;
    }
    
  mT1ClearIntFlag();
}

//=============================================
// Configure the Timer 2 interrupt handler
//=============================================
void __ISR(_TIMER_2_VECTOR, T2_INTERRUPT_PRIORITY) Timer2InterruptHandler(void)
{
    static UINT8 i = 50; // Creates a 50ms delay.
    
    i--;
    if (i == 0)
    {
        Flag_50ms = 1;
    }
    
    Flag_1ms = 1;

  mT2ClearIntFlag();
}

//=============================================
// Configure the Timer 3 interrupt handler
//=============================================
//void __ISR(_TIMER_3_VECTOR, T3_INTERRUPT_PRIORITY) Timer3InterruptHandler(void)
//{   
    //Flag_Calib = 1;

  //mT3ClearIntFlag();
//}

//=============================================
// Configure the Timer 4 interrupt handler
//=============================================
//void __ISR(_TIMER_4_VECTOR, T4_INTERRUPT_PRIORITY) Timer4InterruptHandler(void)
//{
//    Flag_500ms = 1;
//
//  mT4ClearIntFlag();
//}

//=============================================
// Configure the Timer 5 interrupt handler
//=============================================
//void __ISR(_TIMER_5_VECTOR, T5_INTERRUPT_PRIORITY) Timer5InterruptHandler(void)
//{
//  /*
//   * DEVELOPPER CODE HERE
//   */
//
//  mT5ClearIntFlag();
//}


/*******************************************************************************
 ***********************                               *************************
 ********************           UART INTERRUPTS           **********************
 ***********************                               *************************
 *******************************************************************************/



//=============================================
// Configure the UART 6 interrupt handler
//=============================================

void __ISR(_UART_6_VECTOR, U6_INTERRUPT_PRIORITY) Uart6InterruptHandler(void)
{
	// RX interrupt handling
  //===========================================================
	if(INTGetFlag(INT_SOURCE_UART_RX(UART6)))
	{
    // Clear the RX interrupt Flag
    INTClearFlag(INT_SOURCE_UART_RX(UART6));

    /*
     *  DEVELOPPER CODE HERE
     */
	}
  //===========================================================

	// TX interrupt handling
  //===========================================================
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART6)) )
	{
    // Clear the TX interrupt Flag
    INTClearFlag(INT_SOURCE_UART_TX(UART6));

    /*
     *  DEVELOPPER CODE HERE
     */
	}
  //===========================================================
}
//=============================================


//=============================================
// Configure the ADC interrupt handler
//=============================================
void __ISR(_ADC_VECTOR, ADC_INTERRUPT_PRIO) AdcInterruptHandler(void)
{
  Adc.Read();               // Read the enabled channels and puts them in Adc.Var.adcReadValues[]
  INTClearFlag(INT_AD1);    // Clear the ADC conversion done interrupt Flag
  oRotary_Flag = 1;
}
////=============================================


// Function to handle CAN message entries
void Get_CAN_Data(UINT can_channel, void* dst, size_t size)
{
    // The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You could either
    // read the channel in the ISR to clear the event condition or as done
    // here, disable the event source, and set an application flag to
    // indicate that a message has been received. The event can be
    // enabled by the application when it has processed one message.
    // Note that leaving the event enabled would cause the CPU to keep
    // executing the ISR since the CAN_RX_CHANNEL_NOT_EMPTY event is
    // persistent (unless the not empty condition is cleared.)
    CANEnableChannelEvent(CAN1, can_channel, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
    CANRxMessageBuffer* message;
    message = CANGetRxMessage(CAN1, can_channel);
    
    Flag_CAN = 1;
    
    // Copy the CAN data into the destination
    memcpy(dst, &message->data[0], size);
    
    CANUpdateChannel(CAN1, CAN_CHANNEL1);
    CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
}

//================================================
// Configure the CAN1 interrupt handler
//================================================
//void __attribute__((vector(_CAN_1_VECTOR), interrupt(ipl4auto), nomips16)) CAN1InterruptHandler(void)
void __ISR(_CAN_1_VECTOR, CAN1_INT_PRIORITY) Can1InterruptHandler(void)
{
    //CAN_Pitch = 666;
  /*
    // Check if the source of the interrupt is RX_EVENT. This is redundant since
    // only this event is enabled in this example but this shows one scheme for
    // handling events
    if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0)
    {
        // We received a CAN packet
        // This is hacky and not a good idea.
        // the enum for CAN events maps the one for channels, so
        // CAN_CHANNEL1_EVENT == CAN_CHANNEL1 == 1, etc..
        UINT channel = CANGetPendingEventCode(CAN1);
        Get_CAN_Data(channel, CAN_msg_dests[channel], 4);
    }
  */
    // Check if the source of the interrupt is RX_EVENT. This is redundant since
    // only this event is enabled in this example but this shows one scheme for
    // handling events
    if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT)) {

        // Within this, you can check which channel caused the event by using
        // the CANGetModuleEvent() function which returns a code representing
        // the highest priority pending event.
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL1_EVENT) {
            

            // This means that channel 1 caused the event.
            // The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You could either
            // read the channel in the ISR to clear the event condition or as done
            // here, disable the event source, and set an application flag to
            // indicate that a message has been received. The event can be
            // enabled by the application when it has processed one message.
            // Note that leaving the event enabled would cause the CPU to keep
            // executing the ISR since the CAN_RX_CHANNEL_NOT_EMPTY event is
            // persistent (unless the not empty condition is cleared.)

            CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL1);

            Flag_CAN = 1;
             
            CAN_Gear = message->data[0];
            //Sid = message->msgSID;
            
            CANUpdateChannel(CAN1, CAN_CHANNEL1);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL2_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL2, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL2);
            
            Flag_CAN = 1;
             
            
            memcpy((void *) &CAN_Pitch, &message->data[0], 4);
            //CAN_Pitch = (message->data[1] << 8) | message->data[0];
            //CAN_Pitch(1234);
            //CAN_Pitch = 8888;
           
            CANUpdateChannel(CAN1, CAN_CHANNEL2);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL2, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL3_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL3, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL3);
            
            Flag_CAN = 1;
             
            memcpy((void *) &CAN_Mast_Direction, &message->data[0], 4);
           
            CANUpdateChannel(CAN1, CAN_CHANNEL3);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL3, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
            

        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL4_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL4, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL4);
            
            Flag_CAN = 1;
             
            CAN_Mast_Mode = (message->data[1] << 8) | message->data[0];
            
            // 1 = manual mode and 0 = auto mode
            
           
            CANUpdateChannel(CAN1, CAN_CHANNEL4);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL4, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL5_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL5, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL5);
            
            Flag_CAN = 1;
            
            CAN_Calib_Done = 1;
            
            //CAN_MastCalib = (message->data[1] << 8) | message->data[0];
            
            // si message reÁu = calib done
            
           
            CANUpdateChannel(CAN1, CAN_CHANNEL5);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL5, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
            

        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL6_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL6, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL6);
            
            Flag_CAN = 1;
            
            
            //memcpy((void *) &CAN_Turb_RPM, &message->data[0], 4);
            //CAN_Turb_RPM = 45.0f;

           
            CANUpdateChannel(CAN1, CAN_CHANNEL6);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL6, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);

        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL7_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL7, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL7);
            
              
            
            Flag_CAN = 1;
            
            
            memcpy((void *) &CAN_Wind_Speed, &message->data[0], 4);
            
               
           
            CANUpdateChannel(CAN1, CAN_CHANNEL7);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL7, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL8_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL8, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL8);
            
              
            
            Flag_CAN = 1;
            
            
            memcpy((void *) &CAN_Current, &message->data[0], 4);
            
               
           
            CANUpdateChannel(CAN1, CAN_CHANNEL8);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL8, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
         if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL9_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL9, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL9);
            
              
            
            Flag_CAN = 1;
            
            
            memcpy((void *) &CAN_Voltage, &message->data[0], 4);
            
               
           
            CANUpdateChannel(CAN1, CAN_CHANNEL9);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL9, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL10_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL10, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL10);
            
              
            
            Flag_CAN = 1;
            
            
            memcpy((void *) &CAN_Wheel_RPM, &message->data[0], 4);
            //CAN_Wheel_RPM = 21.1f;
            
               
           
            CANUpdateChannel(CAN1, CAN_CHANNEL10);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL10, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
        
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL11_EVENT) 
        {
            
            CANEnableChannelEvent(CAN1, CAN_CHANNEL11, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN_RX_MSG_SID Sid;
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL11);
            
              
            
            Flag_CAN = 1;
            
            
            memcpy((void *) &CAN_Wind_Direction, &message->data[0], 4);
            
               
           
            CANUpdateChannel(CAN1, CAN_CHANNEL11);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL11, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
    }
    
    if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL12_EVENT) 
        {
            CANEnableChannelEvent(CAN1, CAN_CHANNEL12, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL12);
            
            CAN_Acq_Stat ^= 1;
           
            CANUpdateChannel(CAN1, CAN_CHANNEL12);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL12, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
    
        if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL13_EVENT) 
        {
            CANEnableChannelEvent(CAN1, CAN_CHANNEL13, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CANRxMessageBuffer *message;
            message = CANGetRxMessage(CAN1,CAN_CHANNEL13);
            
             CAN_Pitch_Mode = (message->data[1] << 8) | message->data[0];
           
            CANUpdateChannel(CAN1, CAN_CHANNEL13);
            CANEnableChannelEvent(CAN1, CAN_CHANNEL13, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);


        }
    
    if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL14_EVENT)
    {
        CANEnableChannelEvent(CAN1, CAN_CHANNEL14, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
        CANRxMessageBuffer* message;
        message = CANGetRxMessage(CAN1, CAN_CHANNEL14);
        
        // Read torque value
        memcpy((void *) &CAN_Torque, &message->data[0], 4);
        
        CANUpdateChannel(CAN1, CAN_CHANNEL14);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL14, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
    
    if (CANGetPendingEventCode(CAN1) == CAN_CHANNEL15_EVENT)
    {
        CANEnableChannelEvent(CAN1, CAN_CHANNEL15, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
        CANRxMessageBuffer* message;
        message = CANGetRxMessage(CAN1, CAN_CHANNEL15);
        
        // Read loadcell value
        memcpy((void *) &CAN_Loadcell, &message->data[0], 4);
        
        CANUpdateChannel(CAN1, CAN_CHANNEL15);
        CANEnableChannelEvent(CAN1, CAN_CHANNEL15, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
    }
    
    
    // The CAN1 Interrupt flag is  cleared at the end of the interrupt routine.
    // This is because the event source that could have caused this interrupt to
    // occur (CAN_RX_CHANNEL_NOT_EMPTY) is disabled. Attempting to clear the
    // CAN1 interrupt flag when the the CAN_RX_CHANNEL_NOT_EMPTY interrupt is
    // enabled will not have any effect because the base event is still present
  INTClearFlag(INT_CAN1);
  
}