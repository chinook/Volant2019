//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Setup.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the setup of the system. It contains the
//           initialization functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//           Function names can and should be renamed by the user to improve the
//           readability of the code. Also, the LED used for testing errors in
//           TimerInit is a LED on the MAX32 development board. Developpers
//           should test for errors by the means (hardware of software) they
//           judge are the best.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include "Setup.h"
#include "Interrupts.h"


//==============================================================================
//	INIT FUNCTIONS
//==============================================================================


//===========================
//	INIT TIMERS
//===========================
void InitTimer(void)
{

  INT32 timerCounterValue = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Open timers
//%%%%%%%%%%%%%%%%%%%%%%%%%%%
  timerCounterValue = Timer.Open(TIMER_1, 1000, SCALE_MS);   // Open Timer 1 with a period of 1s
  //timerCounterValue = Timer.Open(TIMER_2, 50, SCALE_MS);   // Open Timer 2 with a period of 50 ms
  timerCounterValue = Timer.Open(TIMER_2, 1, SCALE_MS);   // Open Timer 2 with a period of 10 ms
  timerCounterValue = Timer.Open(TIMER_3, 5, SCALE_MS);   // Open Timer 3 with a period of 5 ms. Used for ADC.
//  timerCounterValue = Timer.Open(TIMER_4, 500, SCALE_MS);   // Open Timer 4 with a period of 500 ms
 //timerCounterValue = Timer.Open(TIMER_5, 500, SCALE_US);   // Open Timer 5 with a period of 500 us
  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//	Configure timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Timer.ConfigInterrupt(TIMER_1, TIMER1_INTERRUPT_PRIORITY, TIMER1_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_1 to the values specified in Interrupt.h
  Timer.ConfigInterrupt(TIMER_2, TIMER2_INTERRUPT_PRIORITY, TIMER2_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_2 to the values specified in Interrupt.h
  //Timer.ConfigInterrupt(TIMER_3, TIMER3_INTERRUPT_PRIORITY, TIMER3_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_3 to the values specified in Interrupt.h
//  Timer.ConfigInterrupt(TIMER_4, TIMER4_INTERRUPT_PRIORITY, TIMER4_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_4 to the values specified in Interrupt.h
 // Timer.ConfigInterrupt(TIMER_5, TIMER5_INTERRUPT_PRIORITY, TIMER5_INTERRUPT_SUBPRIORITY); // Sets the priority of the TIMER_5 to the values specified in Interrupt.h

}


//===========================
//	INIT SPI
//===========================
void InitSpi(void)
{
  INT8 err = 0;
  SpiOpenFlags_t oMasterFlags =   SPI_MASTER_MODE
                                | SPI_16_BITS_CHAR
                                | SPI_ENHANCED_BUFFER_MODE
                                | SPI_TX_EVENT_BUFFER_SR_EMPTY
                                | SPI_RX_EVENT_BUFFER_NOT_EMPTY
                                | SPI_SAMPLE_END_CLK
                ;
  err = Spi.Open(SPI3, oMasterFlags, 4e6);   // Open the SPI4 as a master at a bitrate of 5 MHz
  if (err < 0)                // Check for errors
  {
//    Port.C.SetBits(BIT_1);    // Turn on the LD5 on MAX32
  }

  // SPI interrupts not functionnal as of yet
//  Spi.ConfigInterrupt(SPI2, SPI2_INTERRUPT_PRIORITY, SPI2_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI2
//  Spi.ConfigInterrupt(SPI3, SPI3_INTERRUPT_PRIORITY, SPI4_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI3
//  Spi.ConfigInterrupt(SPI4, SPI3_INTERRUPT_PRIORITY, SPI4_INTERRUPT_SUBPRIORITY);  // Configure Interrupt for SPI4
}

//===========================
//	INIT PWM
//===========================
//void InitPwm(void)
//{
//
//  // Open PWM1 using Timer3 with 50% duty cycle and 0% offset
//  Pwm.Open(PWM_1);
//  Pwm.SetDutyCycle  (PWM_1, 50);
//  Pwm.SetPulseOffset(PWM_1,  0);
//
//  // Open PWM2 using Timer3 with 50% duty cycle and 0% offset
//  Pwm.Open(PWM_2);
//  Pwm.SetDutyCycle  (PWM_2, 50);
//  Pwm.SetPulseOffset(PWM_2,  0);
//
//  // Open PWM3 using Timer3 with 50% duty cycle and 0% offset
//  Pwm.Open(PWM_3);
//  Pwm.SetDutyCycle  (PWM_3, 50);
//  Pwm.SetPulseOffset(PWM_3,  0);
//
//  // Open PWM4 using Timer3 with 50% duty cycle and 0% offset
//  Pwm.Open(PWM_4);
//  Pwm.SetDutyCycle  (PWM_4, 50);
//  Pwm.SetPulseOffset(PWM_4,  0);
//
//  // Open PWM5 using Timer3 with 50% duty cycle and 0% offset
//  Pwm.Open(PWM_5);
//  Pwm.SetDutyCycle  (PWM_5, 50);
//  Pwm.SetPulseOffset(PWM_5,  0);
//
//}


//===========================
//	INIT PORTS
//===========================

void InitPorts(void)
{
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Set unused pins as input to protect the pins of the microcontroller
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Port.A.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RA8, RA11-13 non existent
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_9  | BIT_10 | BIT_14 | BIT_15 );

  Port.B.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.C.CloseBits  ( BIT_1  | BIT_2  | BIT_3  | BIT_4      // RC0, RC5-11 non existent
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.D.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  | BIT_10 | BIT_11
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

  Port.E.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RE10-15 non existent
                    | BIT_4  | BIT_5  | BIT_6  | BIT_7
                    | BIT_8  | BIT_9  );

  Port.F.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RF6-7, RF9-11, RF14-15 non existent
                    | BIT_4  | BIT_5  | BIT_8  | BIT_12
                    | BIT_13 );

  Port.G.CloseBits  ( BIT_0  | BIT_1  | BIT_2  | BIT_3      // RG4-5, RG10-11 non existent
                    | BIT_6  | BIT_7  | BIT_8  | BIT_9
                    | BIT_12 | BIT_13 | BIT_14 | BIT_15 );

   /* Setup IO LED */
    Port.B.SetPinsDigitalOut(BIT_8);      // LED_DEBG0
    Port.B.SetPinsDigitalOut(BIT_9);      // LED_DEBG1
    Port.B.SetPinsDigitalOut(BIT_10);     // LED_DEBG2
    Port.B.SetPinsDigitalOut(BIT_11);     // LED_DEBG3
    Port.B.SetPinsDigitalOut(BIT_12);     // LED_DEBG4
    Port.B.SetPinsDigitalOut(BIT_13);     // LED_CAN
    Port.B.SetPinsDigitalOut(BIT_14);     // LED_ERROR
    Port.B.SetPinsDigitalOut(BIT_15);     // LED_STATUS
    
    /* LED ON*/
    //LED_STATUS  = 0;
    //LED_CAN     = 0;
    //LED_ERROR   = 0;
    //LED_DEBG4   = 0;
    //LED_DEBG3   = 0;
    //LED_DEBG2   = 0;
    //LED_DEBG1   = 0;
    //LED_DEBG0   = 0;
    LED_STATUS  = 1;
    LED_CAN     = 1;
    LED_ERROR   = 1;
    LED_DEBG4   = 1;
    LED_DEBG3   = 1;
    LED_DEBG2   = 1;
    LED_DEBG1   = 1;
   // LED_DEBG0   = 1;
    
    
    /* Setup  Input back switch */
    Port.D.SetPinsDigitalIn(BIT_7);      // pushbutton 1 (back)
    Port.D.SetPinsDigitalIn(BIT_8);      // pushbutton 2 (back)
    
    
    /* Setup  Input front switch */
    Port.D.SetPinsDigitalIn(BIT_9);      // pushbutton 3
    Port.D.SetPinsDigitalIn(BIT_10);     // pushbutton 4
    Port.D.SetPinsDigitalIn(BIT_11);     // pushbutton 5
    Port.E.SetPinsDigitalIn(BIT_0);      // pushbutton 6
    Port.E.SetPinsDigitalIn(BIT_1);      // pushbutton 7
    Port.E.SetPinsDigitalIn(BIT_2);      // pushbutton 8
    Port.E.SetPinsDigitalIn(BIT_3);      // pushbutton 9
    
    Port.E.SetPinsDigitalIn(BIT_4);      // pushbutton 10 (emergency brake)
    
    /* Setup  SPI SCn */
    Port.D.SetPinsDigitalIn(BIT_2);       // SDI
    Port.D.SetPinsDigitalOut(BIT_3);      // SD0
    Port.D.SetPinsDigitalOut(BIT_0);      // DRVB_SC
    Port.D.SetPinsDigitalOut(BIT_1);      // CLK

}


//===========================
//	INIT UART
//===========================
void InitUart (void)
{

  UartConfig_t       oConfig      = UART_ENABLE_PINS_TX_RX_ONLY;
  UartFifoMode_t     oFifoMode    = UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY;
  UartLineCtrlMode_t oLineControl = UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1;

  Uart.Open(UART6, BAUD57600, oConfig, oFifoMode, oLineControl);   // Open UART 2 as : 57600 BAUD, 1 stop bit, no parity and 8 bits data, not used as main UART for Skadi
  Uart.EnableTx(UART6);
  Uart.EnableRx(UART6);

//  Uart.ConfigInterrupt(UART6, UART6_INTERRUPT_PRIORITY, UART6_INTERRUPT_SUBPRIORITY);
  
}


//===========================
//	INIT CAN BUS
//===========================
void InitCan(void)
{
  /**Setup par defaut.
   * CAN_CHANNEL0
   * Mode: TX
   * Type: SID
   *
   * CAN_CHANNEL1
   * Mode: RX
   * CAN_FILTER0: 0xC1, this configures the filter to accept with ID 0xC1
   * CAN_FILTER_MASK0: 0x00, Configure CAN1 Filter Mask 0 to comprare no bits
   * */

   Can.Initialize(CAN1, Can1MessageFifoArea, CAN_NB_CHANNELS, CAN_BUFFER_SIZE, FALSE);
   
   
   // Channel 1 - 0x23 would be the GEAR feeback value from a drive board
   Can.SetChannel(CAN1, CAN_CHANNEL1, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL1, CAN_FILTER0, ID_GEAR_VALUE, CAN_FILTER_MASK0,0x7FF);
   // Channel 2 - 0x13 would be the PITCH feeback value from a drive board
   Can.SetChannel(CAN1, CAN_CHANNEL2, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL2, CAN_FILTER1, ID_PITCH_VALUE, CAN_FILTER_MASK0,0x7FF);
   // Channel 3 - 0x32 would be the MAST DIRECTION feeback value from a MAST-drive board
   Can.SetChannel(CAN1, CAN_CHANNEL3, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL3, CAN_FILTER2, MAST_DIRECTION_SID, CAN_FILTER_MASK0,0x7FF);
   // Channel 4 - 0x33 would be the MAST AUTO/MANUAL MODE feeback value from a MAST-drive board
   Can.SetChannel(CAN1, CAN_CHANNEL4, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL4, CAN_FILTER3, MAST_MODE_SID, CAN_FILTER_MASK0,0x7FF);
   // Channel 5 - 0xC2 would be the CALIBRATION DONE feeback
   Can.SetChannel(CAN1, CAN_CHANNEL5, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL5, CAN_FILTER4, CALIB_DONE_SID, CAN_FILTER_MASK0,0x7FF);
   // Channel 5 - 0x34 would be the MAST CALIBRATION DONE feeback value from a MAST-drive board
//   Can.SetChannel(CAN1, CAN_CHANNEL5, 8, RX);
//   Can.SetChannelMask(CAN1, CAN_CHANNEL5, CAN_FILTER4, MAST_CALIB_DONE_SID, CAN_FILTER_MASK0,0x7FF);
   // Channel 6 - 0x21 would be the Wind Turbine RPM value from sensor board
   Can.SetChannel(CAN1, CAN_CHANNEL6, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL6, CAN_FILTER5, 0x21, CAN_FILTER_MASK0,0x7FF);
    // Channel 7 - 0x19 would be the Wind speed value from sensor board
   Can.SetChannel(CAN1, CAN_CHANNEL7, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL7, CAN_FILTER6, 0x19, CAN_FILTER_MASK0,0x7FF);
    // Channel 8 - 0x73 would be the Amp(I) value from the BackPlane
   Can.SetChannel(CAN1, CAN_CHANNEL8, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL8, CAN_FILTER7, 0x73, CAN_FILTER_MASK0,0x7FF);
   // Channel 9 - 0x72 would be the Voltage value from the BackPlane
   Can.SetChannel(CAN1, CAN_CHANNEL9, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL9, CAN_FILTER8, 0x72, CAN_FILTER_MASK0,0x7FF);
   // Channel 10 - 0x22 would be wheel RPM (162,5 cm)
   Can.SetChannel(CAN1, CAN_CHANNEL10, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL10, CAN_FILTER9, 0x22, CAN_FILTER_MASK0,0x7FF);
   // Channel 11 - 0x20 Wind Direction
   Can.SetChannel(CAN1, CAN_CHANNEL11, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL11, CAN_FILTER10, WIND_DIRECTION_ID, CAN_FILTER_MASK0,0x7FF);
   // Channel 12 - 0x25 Acq Toggle acknowledge
   Can.SetChannel(CAN1, CAN_CHANNEL12, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL12, CAN_FILTER11, VOLANT_ACQ_ACK, CAN_FILTER_MASK0,0x7FF); //ernesto
   
   Can.SetChannel(CAN1, CAN_CHANNEL13, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL13, CAN_FILTER12, ID_PITCH_MODE_SID, CAN_FILTER_MASK0,0x7FF); //ernesto
   
   // Torque
   Can.SetChannel(CAN1, CAN_CHANNEL14, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL14, CAN_FILTER13, 0x50, CAN_FILTER_MASK0, 0x7FF);
   
   // Loadcell
   Can.SetChannel(CAN1, CAN_CHANNEL15, 8, RX);
   Can.SetChannelMask(CAN1, CAN_CHANNEL15, CAN_FILTER14, 0x51, CAN_FILTER_MASK0, 0x7FF);
   
   Can.ConfigInterrupt(CAN1, CAN1_INTERRUPT_PRIORITY, CAN1_INTERRUPT_SUBPRIORITY);
   
   //INTEnableInterrupts();
}


//===========================
//	INIT I2C
//===========================
void InitI2C(void)
{
  return;
}


//===========================
//	INIT WATCHDOG TIMER
//===========================
void InitWdt(void)
{
  return;
}


//===========================
//	INIT ADC
//===========================
//void InitAdc(void)
//{
//  // Mode of operation. Uncomment the one you need.
//  //================================================
//  UINT32 samplingClk = ADC_CLK_TMR;     // Timer3 used for sampling
////  UINT32 samplingClk = ADC_CLK_AUTO;    // Internal counter used for sampling
////  UINT32 samplingClk = ADC_CLK_INT0;    // Active transition on INT0 used for sampling
////  UINT32 samplingClk = ADC_CLK_MANUAL;  // Manual sampling.
//  //================================================
//
//  // Hardware config. These are exemples.
//  //================================================
////  UINT32 configHardware = ADC_VREF_AVDD_AVSS      // Vref+ is AVdd and Vref- is AVss
////                        | ADC_SAMPLES_PER_INT_16; // 16 samples/interrupt
//  UINT32 configHardware = ADC_VREF_EXT_AVSS       // Vref+ is External and Vref- is AVss
//                        | ADC_SAMPLES_PER_INT_1; // 1 sample/interrupt => used in manual mode
//  //================================================
//
//  // Port config. These are exemples.
//  //================================================
////  UINT32 configPort = ENABLE_ALL_ANA; // Enable AN0-AN15 in analog mode
////  UINT32 configScan = 0; // Scan all ANs
////  UINT32 configScan = SKIP_SCAN_ALL; // Skip all scans
//
//  UINT32 configPort = ENABLE_AN2_ANA; // Enable AN0, AN4 and AN13 in analog mode
////
//  UINT32 configScan = SKIP_SCAN_AN0
//                    | SKIP_SCAN_AN1
//                    | SKIP_SCAN_AN3
//                    | SKIP_SCAN_AN4
//                    | SKIP_SCAN_AN5
//                    | SKIP_SCAN_AN6
//                    | SKIP_SCAN_AN7
//                    | SKIP_SCAN_AN8
//                    | SKIP_SCAN_AN9
//                    | SKIP_SCAN_AN10
//                    | SKIP_SCAN_AN11
//                    | SKIP_SCAN_AN12
//                    | SKIP_SCAN_AN13
//                    | SKIP_SCAN_AN14
//                    | SKIP_SCAN_AN15; // Don't scan the channels that are not enabled by configPort
//  //================================================
//
//  // Open ADC with parameters above
//  Adc.Open(samplingClk, configHardware, configPort, configScan);
//
//  Adc.ConfigInterrupt(ADC_INTERRUPT_PRIORITY, ADC_INTERRUPT_SUBPRIORITY);
//
//}


//===========================
//	INIT INPUT CAPTURE
//===========================
void InitInputCapture(void)
{
  return;
}


//===========================
//	START INTERRUPTS
//===========================
void StartInterrupts(void)
{

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable timer interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Timer.EnableInterrupt(TIMER_1);
  Timer.EnableInterrupt(TIMER_2);
//  Timer.EnableInterrupt(TIMER_3);
//  Timer.EnableInterrupt(TIMER_4);
//  Timer.EnableInterrupt(TIMER_5);


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable UART interrupts             // Not functionnal yet
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//  Uart.EnableRxInterrupts(UART1);   // Enable RX Interrupts for UART1
//  Uart.EnableTxInterrupts(UART1);   // Enable TX Interrupts for UART1
//
//  Uart.EnableRxInterrupts(UART2);   // Enable RX Interrupts for UART2
//  Uart.EnableTxInterrupts(UART2);   // Enable TX Interrupts for UART2
//
//  Uart.EnableRxInterrupts(UART3);   // Enable RX Interrupts for UART3
//  Uart.EnableTxInterrupts(UART3);   // Enable TX Interrupts for UART3
//
//  Uart.EnableRxInterrupts(UART4);   // Enable RX Interrupts for UART4
//  Uart.EnableTxInterrupts(UART4);   // Enable TX Interrupts for UART4
//
//  Uart.EnableRxInterrupts(UART5);   // Enable RX Interrupts for UART5
//  Uart.EnableTxInterrupts(UART5);   // Enable TX Interrupts for UART5
//
//  Uart.EnableRxInterrupts(UART6);   // Enable RX Interrupts for UART6
//  Uart.EnableTxInterrupts(UART6);   // Enable TX Interrupts for UART6


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable SPI interrupts             // Not functionnal yet
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#ifdef __32MX795F512L__   // SPI1 only on this device
  Spi.EnableRxInterrupts(SPI1);   // Enable RX Interrupts for SPI1
  Spi.EnableTxInterrupts(SPI1);   // Enable TX Interrupts for SPI1
#endif

// Spi.EnableRxInterrupts(SPI2);   // Enable RX Interrupts for SPI2
//  Spi.EnableTxInterrupts(SPI2);   // Enable TX Interrupts for SPI2

//  Spi.EnableRxInterrupts(SPI3);   // Enable RX Interrupts for SPI3
//  Spi.EnableTxInterrupts(SPI3);   // Enable TX Interrupts for SPI3

//  Spi.EnableRxInterrupts(SPI4);   // Enable RX Interrupts for SPI4
//  Spi.EnableTxInterrupts(SPI4);   // Enable TX Interrupts for SPI4


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable ADC interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Adc.EnableInterrupts();   // Works only when not in manual mode

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable CAN interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Can.EnableInterrupt(CAN1);
  
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Enable multi-vector interrupts
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
  INTEnableInterrupts();
  

}

//===========================
//	INIT ADC
//===========================
void InitAdc(void)
{
  // Mode of operation. Uncomment the one you need.
  //================================================
  UINT32 samplingClk = ADC_CLK_TMR;     // Timer3 used for sampling
  //================================================

  // Hardware config. These are exemples.
  //================================================
  UINT32 configHardware = ADC_VREF_AVDD_AVSS      // Vref+ is AVdd and Vref- is AVss
                        | ADC_SAMPLES_PER_INT_1;  // 3 samples/interrupt (we check 3 channels)
  //================================================
  // Port config.
  //================================================

  UINT32 configPort = ENABLE_AN2_ANA; // Change for good channel
  
  UINT32 configScan = SKIP_SCAN_AN0
                    | SKIP_SCAN_AN1
                    | SKIP_SCAN_AN3
                    | SKIP_SCAN_AN4
                    | SKIP_SCAN_AN5
                    | SKIP_SCAN_AN6
                    | SKIP_SCAN_AN7
                    | SKIP_SCAN_AN8
                    | SKIP_SCAN_AN9
                    | SKIP_SCAN_AN10
                    | SKIP_SCAN_AN11
                    | SKIP_SCAN_AN12
                    | SKIP_SCAN_AN13
                    | SKIP_SCAN_AN14
                    | SKIP_SCAN_AN15; // Don't scan the channels that are not enabled by configPort
  //================================================

  // Open ADC with parameters above
  Adc.Open(samplingClk, configHardware, configPort, configScan);

  Adc.ConfigInterrupt(ADC_INTERRUPT_PRIORITY, ADC_INTERRUPT_SUBPRIORITY);
}