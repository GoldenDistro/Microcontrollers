#include <p18f45k22.h>
#include <delays.h>

#pragma config FOSC = INTIO67, PLLCFG = OFF, PRICLKEN = ON, FCMEN = ON, PWRTEN = OFF
#pragma config BOREN = SBORDIS, BORV = 250, WDTEN = OFF, WDTPS = 4096, PBADEN = OFF
#pragma config HFOFST = OFF, MCLRE = EXTMCLR, STVREN = ON, LVP = OFF, DEBUG = ON

unsigned char Dip_Input;
unsigned char Ping_Pong;
unsigned char Dir = 0;
unsigned char counter;
unsigned char binary = 0;

void main (void){
    
	//setup port B as input with pullup
    INTCON2bits.NOT_RBPU = 0; //B pull ups are enabled
    WPUB = 0x3F;		          
    TRISB = 0xFF;		        //set port B as input
    ANSELB = 0x00;		        //set port B as digital
	
	//setup port C as output
    TRISC = 0x00; 		       //set port C as output
    ANSELC = 0x00;		      //set port C as digital
	
	//setup port D as output
	TRISD = 0x00; 		       //set port D as output
    ANSELD = 0x00;		      //set port D as digital
    
    Ping_Pong = 0x01; //one side
	
	while (1){	
		Dip_Input = (PORTB & 0x30)>>4;  //
		//PORTC = 0x00;
		switch( Dip_Input) 
        {
			case 0:
			while(binary < 0xFF){ //literally just adding 16 to cycle through the last 4 LEDs
                  binary = binary + 1;
                  PORTD = binary; //assuming PORTB connected to LEDs, as 
                                //digital output
                  PORTC = binary;
                  Delay10KTCYx(2);
            }
            binary = 0;
            
        	break;

    		case 1:
               PORTC = 0xFF;
               PORTD = 0xFF;
               Delay10KTCYx(5);
               PORTD = 0x7F; 
               Delay10KTCYx(5);
			break;

			case 2:
               if(Ping_Pong == 0x80){ //turn directions
                  Dir = 0x00;
               }
               else if (Ping_Pong == 0x01){
                  Dir = 0x01;
               }
	            if(Dir == 0x01){
                  Ping_Pong = Ping_Pong<<1;
               }
               else if(Dir == 0x00){
                  Ping_Pong = Ping_Pong>>1;
               }

	            PORTC = Ping_Pong;
               PORTD = 0x00;
               Delay10KTCYx(5);
			break;

			case 3:
                 //no defined case
			break;

			default:  
            PORTD = 00111111; //error, all LEDs on
		}
    }
}