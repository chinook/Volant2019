//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : Setup.h
// Author  : Frederic Chasse
// Date    : 2015-01-03
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the setup header for the system. It contains function
//           prototypes as well as macros and the public structure of
//           ChinookLib functions.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : For ChinookLib to be useable, it must be cloned on your hard
//           drive so the path
//               "..\..\..\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
//           references an existing file.
//
//           Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#ifndef __SETUP_H__
#define	__SETUP_H__

//%%%%%%%%%%%%%%%%%%%%%%%%%
// ChinookLib files
//%%%%%%%%%%%%%%%%%%%%%%%%%
//#include "C:\Users\Romi\Documents\GitHub\ChinookLib\ChinookLib.X\headers\ChinookLib.h"
#include "ChinookLib.h"

//==============================================================================
// ChinookLib functions
//==============================================================================
struct sChinook Chinook;            // Contains all ChinookLib functions

//==============================================================================
// State Machine public functions prototypes
//==============================================================================
void InitTimer        (void);
void InitPwm          (void);
void InitPorts        (void);
void InitCan          (void);
void InitSpi          (void);
void InitUart         (void);
void InitI2C          (void);
void InitWdt          (void);
void InitAdc          (void);
void InitInputCapture (void);
void StartInterrupts  (void);

//==============================================================================
// Define OUTPUT LED
//==============================================================================
#define LED_STATUS        LATBbits.LATB8       // LAT used for 
#define LED_ERROR         LATBbits.LATB9
#define LED_CAN           LATBbits.LATB10
#define LED_DEBG5         LATBbits.LATB15
#define LED_DEBG4         LATBbits.LATB15
#define LED_DEBG3         LATBbits.LATB13
#define LED_DEBG2         LATBbits.LATB12
#define LED_DEBG1         LATBbits.LATB11

//==============================================================================
// Define INPUT SWITCH
//==============================================================================
#define SW1             PORTEbits.RE2   // S7_3 btn 3 **
#define SW2             PORTDbits.RD8   // Not used
#define SW3             PORTDbits.RD9   // Not used
#define SW4             PORTDbits.RD11  // btn bas gauche
#define SW5             PORTDbits.RD9   // S5_3
#define SW6             PORTDbits.RD10  // S5_4
#define SW7             PORTEbits.RE1   // Not used
#define SW8             PORTEbits.RE4   // S2 S4
#define SW9             PORTDbits.RD11  // S5_5 btn bas gauche
#define SW10            PORTEbits.RE1   // S7_2 btn 2 **
#define SW11            PORTDbits.RD8   // S5_2
#define SW12            PORTDbits.RD7   // S5_1
#define SW13            PORTEbits.RE3   // S7_4 btn 4 ***
#define SW14            PORTEbits.RE0   // S7_1 btn 1 **


//==============================================================================
// Define SPI PINOUT
//==============================================================================
#define SPI_CS          LATDbits.LATD0
#define SPI_CLK         LATDbits.LATD1
#define SPI_I           LATDbits.LATD2
#define SPI_O           LATDbits.LATD3


//==============================================================================
// Define INPUT CONTROL DRIVE A
//==============================================================================
//#define DRVA_FLT          LATDbits.LATD6
//#define DRVA_STALL        LATDbits.LATD5

//==============================================================================
// Define OUTPUT CONTROL DRIVE B
//==============================================================================
//#define DRVB_RESET        LATGbits.LATG2
//#define DRVB_SLEEP        LATDbits.LATD0
//#define DRVB_DIR          LATEbits.LATE0
//#define DRVB_STEP         LATCbits.LATC15
//#define DRVB_BIN1         LATDbits.LATD3
//#define DRVB_BIN2         LATDbits.LATD1

//==============================================================================
// Define INPUT CONTROL DRIVE B
//==============================================================================
//#define DRVB_FLT          LATDbits.LATD5
//#define DRVB_STALL        LATCbits.LATC15

//==============================================================================
// Define OUTPUT SPI SCn
//==============================================================================
#define DRVB_SC           LATCbits.LATC13
#define DRVA_SC           LATCbits.LATC14

//==============================================================================
// Define INPUT LIMIT SWITCH
//==============================================================================
//#define LIMIT_SW          PORTDbits.RD10

//==============================================================================
// Variable declarations
//==============================================================================
struct sChinook Chinook; // ChinookLib functions

#define CAN_NB_CHANNELS     6 // 2 CAN channels will be used
#define CAN_BUFFER_SIZE     8

BYTE Can1MessageFifoArea [ CAN_NB_CHANNELS     // Space used by CAN
                         * CAN_BUFFER_SIZE
                         * CAN_TX_RX_MESSAGE_SIZE_BYTES
                         ];

// CAN communication MACRO
#define SEND_ID_TO_BACKPLANE     Can.SendByte(CAN1, BACKPLANE_BOARD_SID , VOLANT_BOARD_SID)

#endif	/* __SETUP_H__ */