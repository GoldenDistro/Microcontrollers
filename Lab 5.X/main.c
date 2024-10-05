#include <p18cxxx.h>
#include "my_xlcd.h"
#include <delays.h>
#include <stdlib.h>

#pragma config FOSC = INTIO67, PLLCFG = OFF, PRICLKEN = ON,FCMEN = ON, PWRTEN = ON
#pragma config BOREN = SBORDIS, BORV = 250, WDTEN = OFF, WDTPS = 4096, PBADEN = OFF
#pragma config HFOFST = OFF, MCLRE = EXTMCLR, STVREN = ON, LVP = OFF, DEBUG = ON

void DelayFor18TCY( void ){
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
Nop();
}
void DelayPORXLCD (void){
Delay1KTCYx(60); // Delay of 15ms
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (15ms * 16MHz) / 4
// Cycles = 60,000
return;
}
void DelayXLCD (void){
Delay1KTCYx(500); // Delay of 5ms if 20 or 125ms if 500
// Cycles = (TimeDelay * Fosc) / 4
// Cycles = (5ms * 16MHz) / 4
// Cycles = 20,000
return;
}


unsigned long frequency, count, low, high;

void main( void ){	
    //configure the Oscillator for 1Mhz internal oscillator operation
	OSCCON2 = 0x00; //no 4X PLL
	OSCCON = 0x30;  // 1MHZ
    ANSELC = 0x00; //LCD
    TRISC = 0x00;
    ADCON0 = 0b00000001;
    //ADCON1 = 0b00000000; for 5V reference
    ADCON1 = 0b00001000;
    ADCON2 = 0b10110110;
    ANSELB = 0x00;
    
    VREFCON0 = 0b11110000;


    TRISA = 0x10; //RA4
    ANSELA = 0x00;
    //T0CON = 0b10101000; //part 1
    T0CON = 0b10001000; //part 2

    //WriteCmdXLCD(0x06);
    OpenXLCD(FOUR_BIT & LINES_5X7);
    WriteCmdXLCD(SHIFT_DISP_LEFT);
    
    SetDDRamAddr(0x00);
    putrsXLCD("Frequency is");
    SetDDRamAddr(0x46);
    putrsXLCD("Hz");

    while(1){
        /*
        //part 1
        TMR0H = 0x00; //clear upper timer
        TMR0L = 0x00; //clear lower timer
        Delay10KTCYx(25); //delay for 1 second
        low = TMR0L;	//read lower timer
        high = TMR0H; //read higher timer
        frequency = (high << 8) + low;
        */
        
        //part 2
        while(PORTAbits.RA4); //TMR0 is inhibited for 2 cycles following write
        while(!PORTAbits.RA4);
        TMR0H = 0x00; //clear upper timer
        TMR0L = 0x00; //clear lower timer
        while(PORTAbits.RA4);
        while(!PORTAbits.RA4);
        low = TMR0L; //read lower timer
        high = TMR0H; //read higher timer
        count = (high << 8) + low;
        frequency = 250000/count;
        

        //printing used in both parts
        SetDDRamAddr(0x40); //begin writing read frequency
        putcXLCD(frequency/10000 + 48); //10 thousands place
        putcXLCD((frequency%10000)/1000 + 48); //1 thousand place
        putcXLCD((frequency%1000)/100 + 48); //hundreds place
        putcXLCD((frequency%100)/10 + 48); //tens place
        putcXLCD(frequency%10 + 48); //ones place
    }
}