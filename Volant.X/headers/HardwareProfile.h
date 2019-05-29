//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : HardwareProfile.h
// Author  : Frederic Chasse
// Date    : 2015-02-25
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This header file defines the configuration bits of the
//           PIC32MX795F512L.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef __HARDWAREPROFILE_H__
#define	__HARDWAREPROFILE_H__

#include <xc.h>

// DEVCFG3
// USERID = No Setting
#ifndef __32MX320F128H__    // Uno32 doesn't have these settings
#pragma config FSRSSEL    = PRIORITY_7  // SRS Select (SRS Priority 7)  (SRS = Shadow Registers)
#pragma config FMIIEN     = ON          // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO     = ON          // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config FCANIO     = ON          // CAN I/O Pin Select (Default CAN I/O)
#pragma config FUSBIDIO   = OFF         // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO  = OFF         // USB VBUS ON Selection (Controlled by USB Module)
#endif

// DEVCFG2
#pragma config FPLLIDIV   = DIV_2       // PLL Input Divider (2x Divider)
#pragma config FPLLMUL    = MUL_20      // PLL Multiplier (20x Multiplier)

#ifndef __32MX320F128H__    // Uno32 doesn't have these settings
#pragma config UPLLIDIV   = DIV_12      // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN     = OFF         // USB PLL Enable (Disabled and Bypassed)
#endif

#pragma config FPLLODIV   = DIV_1       // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#if defined __32MX795F512L__ || __32MX320F128H__  // Chipkit MAX32 or Uno32
#pragma config FNOSC      = PRIPLL                // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#elif defined __32MX795F512H__                    // Chinook boards
#pragma config FNOSC = FRCPLL                     // Oscillator Selection Bits (Fast RC Osc with PLL)
#endif

#pragma config FSOSCEN    = OFF                   // Secondary Oscillator Enable (Disabled)
#pragma config IESO       = ON                    // Internal/External Switch Over (Enabled)

#if defined __32MX795F512L__ || __32MX320F128H__  // Chipkit MAX32 or Uno32
#pragma config POSCMOD    = HS                    // Primary Oscillator Configuration (HS osc mode)
#elif defined __32MX795F512H__                    // Chinook boards
#pragma config POSCMOD = OFF                      // Primary Oscillator Configuration (Primary osc disabled)
#endif

#pragma config OSCIOFNC   = ON                   // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV     = DIV_8                 // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM      = CSDCMD                // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS      = PS1048576             // Watchdog Timer Postscaler (1:1048576)
#pragma config FWDTEN     = OFF                   // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config DEBUG      = OFF         // Background Debugger Enable (Debugger is disabled)
#pragma config ICESEL     = ICS_PGx2    // ICE/ICD Comm Channel Select (ICE EMUC2/EMUD2 pins shared with PGC2/PGD2)
#pragma config PWP        = OFF         // Program Flash Write Protect (Disable)
#pragma config BWP        = OFF         // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP         = OFF         // Code Protect (Protection Disabled)


#endif	/* __HARDWAREPROFILE_H__ */

