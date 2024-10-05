#include <p18cxxx.h>   //* declarations */
#include <math.h>
unsigned char  counter = 128;
int x;
short long y;
long z;
float a,b;
void main (void)
{	
	ANSELA=0x00;
	TRISA = 0x00; //Make Port A a Digital Output
	while ( 1 )
	{		
		PORTA = counter;
      		counter++;

		x = x+counter;
		x = x*counter;
		x = x/counter;

		y = y+counter;
		y = y*counter;
		y = y/counter;

		z = z+counter;
		z = z*counter;
		z = z/counter;

		a = a+counter;
		a = a*counter;
		a = a/counter;

		b = sin(a);
 	 }	
}
