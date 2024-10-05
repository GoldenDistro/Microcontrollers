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

//from part 2
signed int counter = 0;
unsigned char buffer[16];

//from part 3 
unsigned char hour = 11, minute = 59, second = 56, isAM = 0, i = 0;

//extra credit
unsigned char custom = 0x01;

void main( void ){	
//configure the Oscillator for 1Mhz internal oscillator operation
	OSCCON2 = 0x00; //no 4X PLL
	OSCCON = 0x30;  // 1MHZ

ANSELC = 0x00; //digital / disable analog
TRISC = 0x00; //input

//WriteCmdXLCD(0x06); //Im not sure where to put this, since the program infinitely loops and rewrites ?Hello World? anyway
OpenXLCD(FOUR_BIT & LINES_5X7);
WriteCmdXLCD(SHIFT_DISP_LEFT);

//part 2
//SetDDRamAddr(0x00); //first line, first character
//putrsXLCD("The Counters");
//SetDDRamAddr(0x40); //second line, first character
//putrsXLCD("value is: "); //initials

//part 3
SetDDRamAddr(0x00);
putrsXLCD("Time");
SetDDRamAddr(0x07);
putcXLCD(':');
SetDDRamAddr(0x0A);
putcXLCD(':');
SetDDRamAddr(0x40);
putrsXLCD("Sept 20th 2024");

//extra credit
SetCGRamAddr(0x08);
WriteDataXLCD(0x1B);
WriteDataXLCD(0x1B);
WriteDataXLCD(0x00);
WriteDataXLCD(0x1F);
WriteDataXLCD(0x11);
WriteDataXLCD(0x11);
WriteDataXLCD(0x0E);
WriteDataXLCD(0x00);

SetDDRamAddr(0x4F);
putcXLCD(custom);

while (1) {
    //part 1
    //SetDDRamAddr(0x00); //first line, first character
    //putrsXLCD("Hello World");
    //SetDDRamAddr(0x40); //first line, first character
    //putrsXLCD("AG AV");
    
    //part 2
    //SetDDRamAddr(0x4A); //second line, first character
    //itoa(counter, buffer);
    //putsXLCD(buffer); //initials
    //counter = counter + 1;
    //DelayXLCD();
    
    //part 3
    SetDDRamAddr(0x05);
    putcXLCD(hour/10+48);
    SetDDRamAddr(0x06);
    putcXLCD(hour%10+48);
    
    SetDDRamAddr(0x08);
    putcXLCD(minute/10+48);
    SetDDRamAddr(0x09);
    putcXLCD(minute%10+48);
    
    SetDDRamAddr(0x0B);
    putcXLCD(second/10+48);
    SetDDRamAddr(0x0C);
    putcXLCD(second%10+48);

      
    SetDDRamAddr(0x0E); 
    if(isAM%2){
        putrsXLCD("AM");
    }
    else{
        putrsXLCD("PM");
    }
    
    
    second = second + 1;
    if(second == 60){
        second = 0;
        minute = minute+1;
    }
    if(minute == 60){
        minute = 0;
        hour = hour+1;
    }
    if(hour == 12 && minute == 0 && second == 0){
        isAM = isAM + 1;
    }
    if(hour == 13 && minute == 0 && second == 0){
        hour = 1;
    }
    
    Delay1KTCYx(500);
}
}
