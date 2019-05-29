//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Chinook Project Template
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : StateFunctions.c
// Author  : Frederic Chasse
// Date    : 2015-01-03
// Modified: Gabriel Jean-Louis
// Date    : 2017-06-23
// Modified: Ernesto Castaldo et Marc Beaulieu
// Date    : 2019-03-29
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This is the C file for the functions of the state machine of
//           the system.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : Function names can and should be renamed by the user to improve the
//           readability of the code.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


#include "StateFunctions.h"

#include <math.h>

sSwitchesToRead_t switches = 
{
    .bits[0]   = BIT_2,
    .bits[1]   = BIT_0,    //Not used
    .bits[2]   = BIT_0,    //Not used
    .bits[3]   = BIT_0,    //Not used
    .bits[4]   = BIT_9,
    .bits[5]   = BIT_10,
    .bits[6]   = BIT_0,    //Not used
    .bits[7]   = BIT_4,
    .bits[8]   = BIT_11,
    .bits[9]   = BIT_1,
    .bits[10]  = BIT_8,
    .bits[11]  = BIT_7,
    .bits[12]  = BIT_3,
    .bits[13]  = BIT_0,

    .ports[0]  = IOPORT_E,
    .ports[1]  = IOPORT_D,
    .ports[2]  = IOPORT_D,
    .ports[3]  = IOPORT_D,
    .ports[4]  = IOPORT_D,
    .ports[5]  = IOPORT_D,
    .ports[6]  = IOPORT_D,
    .ports[7]  = IOPORT_E,
    .ports[8]  = IOPORT_D,
    .ports[9]  = IOPORT_E,
    .ports[10] = IOPORT_D,
    .ports[11] = IOPORT_D,
    .ports[12] = IOPORT_E,
    .ports[13] = IOPORT_E,
};

//==============================================================================
// MAX_Write writes the DATA in the register REG of the MAX6954 via SPI
//==============================================================================
void MAX_Write(UINT16 REG, unsigned char DATA)
{
    UINT16 data = (REG << 8 | DATA) <<1;
    
    while(SpiChnIsBusy(SPI3+1));
    SPI_CS = 0;
    Spi.SendCharacter(SPI3, data);
    while(SpiChnIsBusy(SPI3+1));
    SPI_CS = 1;
 
}
//==============================================================================
// MAX_init sets all the proper init values in the registers
//==============================================================================
void MAX_init()
{    
    // MAX_Write(REG_DECODE_MODE, 0x1F);             // First 10 digit in decode mode and 11/12(LED array) in non decode
    MAX_Write(REG_DECODE_MODE, NO_DECODE);
    MAX_Write(REG_SCANLIMIT, SCAN_LIMIT);           // Limits the scanning to 12 digits
    MAX_Write(REG_CONFIG, CONFIG);                  // 
    MAX_Write(REG_GLOBAL_INTENSITY, BRIGHTNESS);    // Sets global startup intensity to halfway
    MAX_Write(REG_DIGIT_TYPE, DISP_TYPE);           // Display 7 to 0 are 7 segment displays.
    
    // Init the 4 led displays with their 7seg address values
    display1.numDigits = 4;
    display1._7seg_displays[0] = 0x20;
    display1._7seg_displays[1] = 0x28;
    display1._7seg_displays[2] = 0x21;
    display1._7seg_displays[3] = 0x29;
    display2.numDigits = 2;
    display2._7seg_displays[0] = 0x2D;
    display2._7seg_displays[1] = 0x2E;
    display3.numDigits = 4;
    display3._7seg_displays[0] = 0x2B;
    display3._7seg_displays[1] = 0x24;
    display3._7seg_displays[2] = 0x2C;
    display3._7seg_displays[3] = 0x25;
    display4.numDigits = 3;
    display4._7seg_displays[0] = 0x22;
    display4._7seg_displays[1] = 0x2A;
    display4._7seg_displays[2] = 0x23;
}

/******************************************************************************
 * Will check every switch to verify if any of them has changed state
 * ***************************************************************************/
UINT8 CheckSW()
{
    UINT8 iSwitch = 0;
    UINT8 ChangeDetected = 0;
    for (iSwitch = 0; iSwitch < 14; iSwitch++)
    {
        if ( !(PORTReadBits(switches.ports[iSwitch], switches.bits[iSwitch])) && !MemoSW[iSwitch])
        {
            MemoSW[iSwitch] = 1;
            ChangeDetected = 1;
        }  
     
        else if((PORTReadBits(switches.ports[iSwitch], switches.bits[iSwitch])) && MemoSW[iSwitch])
        {
            MemoSW[iSwitch] = 0;
            ChangeDetected = 1;
        }
    }   
    return ChangeDetected;
}

//===============================================================
// Name       : ProceesSW
// Purpose    : The conversion mode is triggered by an event on the switches or
//              on the CANbus.
//              All the calculation and value modification shall be done in the state.
//              If needed, sending a CAN message shall also be done here.
//===============================================================
void ProcessSW(void)
{
    TabSW = MemoSW[13]<<13 | MemoSW[12]<<12 | MemoSW[11]<<11 | MemoSW[10]<<10 | MemoSW[9]<<9 | MemoSW[8]<<8 | MemoSW[7]<<7 | MemoSW[6]<<6 | MemoSW[5]<<5 | MemoSW[4]<<4 | MemoSW[3]<<3 | MemoSW[2]<<2 | MemoSW[1]<<1 | MemoSW[0];
    
    if (SW5 != 1)
    { 
        LED_STATUS =0;
        
        while(SW5 != 1);
       
        Disp1++;
        if (Disp1 >= MAX_VALUE_SPEED_DISP)
        {
            Disp1 = WHEEL_SPEED;
        }
    }
    LED_STATUS =1;
    
    if (SW6 != 1)
    {
        LED_STATUS = 0; 
        
        while(SW6 != 1);
        Disp3++;
        if (Disp3 >= MAX_VALUE_RPM_DISP)
        {
            Disp3 = TURB_RPM;
        }
    }
    LED_STATUS = 1;
    
    if (SW9 != 1)
    {
        LED_STATUS = 0; 
        
        while(SW9 != 1);
        Disp2++;
        if (Disp2 >= MAX_VALUE_DISPLAY4)  // This is bad, much easier with %
        {
            Disp2 = EFFICACITE;
        }
        
        // Send a minus pitch message on CAN
        //Can.SendData(CAN1, 0x1, 0x1);
    }
    LED_STATUS = 1;
    
    /*if (HORN_BUTTON == (TabSW & HORN_BUTTON))
    {
        SID_CAN_BUFFER = ID_PITCH_MODE_SID;
        MSG_CAN_BUFFER = EMPTY_CAN_MSG;
        CAN_BUFFER_INCREASE;
    }*/

    if (TabSW != Previous_TabSW)
    {
        Previous_TabSW = TabSW;
        SID_CAN_BUFFER = VOLANT_SW_MSG;
        if(CAN_Pitch_Mode == PITCH_AUTO_MODE)
        {
            /* While the Pitch drive is in auto mode, inhibit the steering
             * wheel's pitch commands */
//            MSG_CAN_BUFFER = (TabSW & ~(PITCH_PLUS_BUTTON | PITCH_MINUS_BUTTON));
        }
        else
        {
            MSG_CAN_BUFFER = TabSW;
        }
        CAN_BUFFER_INCREASE;
    } 
    
    if (MAST_MODE_BUTTON == (TabSW & MAST_MODE_BUTTON))
    {
        SID_CAN_BUFFER = VOLANT_MAST_MODE;
        MSG_CAN_BUFFER = EMPTY_CAN_MSG;
        CAN_BUFFER_INCREASE;
    }
}

//===============================================================
// Name       : set_led_array()
// Purpose    : Will refresh the LED array.
// Return     : No return value.
//===============================================================
void Refresh_led_array(void)
{
    MAX_Write(LED_ARRAY_FIRST8, led_array.F8);
    MAX_Write(LED_ARRAY_LAST8, led_array.L5);
}

//===============================================================
// Name       : refresh_displays()
// Purpose    : Will refresh every 7 segment and LED on the board.
// Return     : No return value.
//===============================================================
void Display_data(void)
{
    // Refresh the displays
    Refresh_led_array();
    
    // Will choose what to display on Display 1 (speed)
    switch (Disp1)
        {
            case WHEEL_SPEED:
                Display_print_float_fixed(&display1, CAN_Wheel_RPM, 2);
                break;
                
            case WIND_DIR:
                Display_print_float_fixed(&display1, (float)CAN_Wind_Direction, 2);
                break;

            case WIND_SPEED:
                Display_print_float_fixed(&display1, CAN_Wind_Speed, 1);
                break;
                
            case CURRENT:
                Display_print_float_fixed(&display1, CAN_Current, 2);
                break;
        }
        
        
        switch (Disp2)
        {
            case VEHICULE_SPEED:
                Display_print_float_fixed(&display4, 60.4f, 1);//calcule a faire avec le rayon de la roue
                break;

            case EFFICACITE:
                Display_print_float_fixed(&display4, 125, 2);
                break;
                
            case TORQUE:
                Display_print_float_fixed(&display4, CAN_Torque, 1);
                break;
                
            case LOADCELL:
                Display_print_float_fixed(&display4, CAN_Loadcell, 1);
                break;
        }
        
        // Will choose what to display on Display 3
        switch (Disp3)
        {
            case TURB_RPM:
                Display_print_float_fixed(&display3, CAN_Turb_RPM, 3);
                break;

            case PITCH_ANGLE:
                Display_print_float_fixed(&display3, CAN_Pitch, 3);
                break;
                
            case MAST_DIR:
                Display_print_float_fixed(&display3, CAN_Mast_Direction, 2);
                break;

//            case WIND_DIR:
//                DisplayWindDir((INT16)CAN_Wind_Direction);
//                break;
                
            case VOLTAGE:
                Display_print_float_fixed(&display3, CAN_Voltage, 2);
                break;
        }
        
        // Display the gear we are on
        Display_print_float(&display2, 17);
}



//
// The following new functions are helpers to display data onto the LED displays
//

void Display_print_float(LedDisplay* display, float value)
{
    float position = pow(10, (int)(log(value)/log(10.0f)));
    float valueToRemove = 0;
    int decimal_position = -1;
    int decimal_set = 0;
    
    // Store the digits in an array
    char digits[display->numDigits];
    
    int i;
    for(i = 0; i < display->numDigits; ++i)
    {
        char digit = (int)((value - valueToRemove) / position);
            
        digits[i] = digit;
        
        // Advance the position
        valueToRemove += (float)digit * position;
        position /= 10.0f;
        
        if(position < 1.0f && !decimal_set)
        {
            decimal_position = i;
            decimal_set = 1;
        }
    }
    
    // Perform the print using the digits
    Display_print_digits(display, digits, decimal_position);
}

void Display_print_float_fixed(LedDisplay* display, float value, int decimal_position)
{
    float position = pow(10.0f, decimal_position);
    
    float valueToRemove = 0;
    
    // Store the digits in an array
    char digits[display->numDigits];
    
    int i;
    for(i = 0; i < display->numDigits; ++i)
    {
        char digit = (int)((value - valueToRemove) / position);
            
        digits[i] = digit;
        
        // Advance the position
        valueToRemove += (float)digit * position;
        position /= 10.0f;
    }
    
    // Perform the print using the digits
    Display_print_digits(display, digits, decimal_position);
}

void Display_print_dec(LedDisplay* display, int value)
{
    int position = pow((float)10, (float)display->numDigits-1);
    int valueToRemove = 0;
    
    // Store the digits in an array
    char digits[display->numDigits];
    
    int i;
    for(i = 0; i < display->numDigits; ++i)
    {
        unsigned digit = ((unsigned)value - (unsigned)valueToRemove) / (unsigned)position;
        digits[i] = (char)digit;
        
        // Advance the position
        valueToRemove += digit * position;
        position /= 10.0;
    }
    
    // Perform the print using the digits
    Display_print_digits(display, digits, 4);
}

void Display_print_digits(LedDisplay* display, char digits[4], int decimal_location)
{   
    int hadDigit = 0;
    
    int i = 0;
    for(i = 0; i < display->numDigits; ++i)
    {
        // Do not display leading zeros UNLESS its at position of the decimal place
        if(digits[i] == 0 && i != decimal_location && !hadDigit)
        {
            MAX_Write(display->_7seg_displays[i], 0);
            continue;
        }
        
        // Check for the decimal dot
        int decimal = 0;
        if(decimal_location == i && i != display->numDigits-1 && decimal_location < display->numDigits-1)
        {
            decimal = DP_7SEG;
        }
        
        if(digits[i] != 0)
            hadDigit = 1;
        
        // Write the digit to the display
        MAX_Write(display->_7seg_displays[i], i_to_7seg[digits[i]] + decimal);
    }
}