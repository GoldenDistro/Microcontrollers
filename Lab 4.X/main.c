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



long int voltage;
unsigned long result;
char ones, tenths, hundreths, thousandths;

char progressBar;
char i = 0;

//extra credit
unsigned char custom = 0x01;

void main( void ){	
//configure the Oscillator for 1Mhz internal oscillator operation
	OSCCON2 = 0x00; //no 4X PLL
	OSCCON = 0x30;  // 1MHZ
    TRISA = 0x00;
    ANSELB = 0x00;
    TRISC = 0x00;
    ANSELC = 0x00;
    ADCON0 = 0b00000001;
    //ADCON1 = 0b00000000; for 5V reference
    ADCON1 = 0b00001000;
    ADCON2 = 0b10110110;
    
    VREFCON0 = 0b11110000;
    
    ANSELC = 0x00; //digital / disable analog
    TRISC = 0x00; //input

//WriteCmdXLCD(0x06);
OpenXLCD(FOUR_BIT & LINES_5X7);
WriteCmdXLCD(SHIFT_DISP_LEFT);

SetDDRamAddr(0x00);
putrsXLCD("Voltage is: ");
SetDDRamAddr(0x47);
putrsXLCD("Volts");

//extra credit. There was no solid character for the battery in CGRAM, so made one.
SetCGRamAddr(0x08);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x1F);

while (1) {
    while(BusyXLCD());
    ADCON0bits.GO = 1;
    Delay1KTCYx(80);
    while(ADCON0bits.GO);
    result = ADRES;
    //voltage = result*488281; //for 5V reference
    voltage = result*400000; //for 4.096V reference
    
    SetDDRamAddr(0x40);
    putcXLCD((voltage/100000)/1000+48);
    putcXLCD('.');
    putcXLCD(((voltage/100000)%1000)/100+48);
    putcXLCD(((voltage/100000)%100)/10+48);
    putcXLCD((voltage/100000)%10+48);
    
    //extra credit battery
    progressBar = (voltage/100000)/1000; //lose data to character
    SetDDRamAddr(0x0B);
    for(i = 0; i < 4; i++){ //wipe characters
        putcXLCD(' ');
    }
    SetDDRamAddr(0x0B);
    for(i = 0; i < progressBar; i++){ //print bar, max 4V
        putcXLCD(custom);
    }
}

}