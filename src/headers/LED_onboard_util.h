#include "TM4C123GH6PM.h"
#define LOW 	0x0000000A

extern float min;
extern float max;
extern float curr_temp;

void init_led(void){
	volatile int *NVIC_EN0 = (volatile int*) 0xE000E100;
	volatile int *NVIC_PRI4 = (volatile int*) 0xE000E410;
	SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	
  GPIOF->DIR			|= 0x0E; //set PF2 as output
  GPIOF->AFSEL		&= (0xFFFFFFF0);  // Regular port function
	GPIOF->PCTL			&= 0xFFFF000F;  // No alternate function
	GPIOF->AMSEL		=0; //Disable analog
	GPIOF->DEN			|=0x0E; // Enable PF2-PF3-PF4 pin as a digital pin
	GPIOF->DIR       |= 0x0E; //set LED pins as a digital output pin
  
	
	SYSCTL->RCGCTIMER	|=0x01; // Start timer0
	__ASM("NOP");
	__ASM("NOP");
	__ASM("NOP");
	TIMER0->CTL			&=0xFFFFFFFE; //Disable timer during setup
	TIMER0->CFG			=0x04;  //Set 16 bit mode
	TIMER0->TAMR		=0x02; // set to periodic, count down
	TIMER0->TAILR		=LOW; //Set interval load as LOW
	TIMER0->TAPR		=64; // Divide the clock by 16 to get 1us
	TIMER0->IMR			=0x01; //Enable timeout intrrupt	
	
	//Timer0A is interrupt 19
	//Interrupt 16-19 are handled by NVIC register PRI4
	//Interrupt 19 is controlled by bits 31:29 of PRI4
	*NVIC_PRI4 &=0x00FFFFFF; //Clear interrupt 19 priority
	*NVIC_PRI4 |=0x40000000; //Set interrupt 19 priority to 2
	
	//NVIC has to be neabled
	//Interrupts 0-31 are handled by NVIC register EN0
	//Interrupt 19 is controlled by bit 19
	*NVIC_EN0 |=0x00080000;
	
	//Enable timer
	TIMER0->CTL			 |=0x03; // bit0 to enable and bit 1 to stall on debug
	return;
}

void TIMER0A_Handler (void){	//TIMER0 is used for arranging the leds intensity. It can be used PWM instead of it, but it is much simpler for me.
	
	if(curr_temp<min){
		heating_pad_open();
		int x=abs(curr_temp-min);
		GPIOF->DATA  &= 2;
		if(x ==0)TIMER0->TAILR = LOW;
		
		else if(x<=1){
			if((GPIOF->DATA & 2) ==2)TIMER0->TAILR = (20*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x<=2){
			if((GPIOF->DATA & 2) ==2)TIMER0->TAILR = (40*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x<=3){
			if((GPIOF->DATA & 2) ==2)TIMER0->TAILR = (60*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else{
			if((GPIOF->DATA & 2) ==2)TIMER0->TAILR = (80*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		GPIOF->DATA  ^= 2;
	}
	
	else if(curr_temp>min && curr_temp<max){
		if(curr_temp < ((min+max)/2))heating_pad_open();
		else heating_pad_close();
		int x = abs(curr_temp-max);
		GPIOF->DATA  &= 8;
		
		if(x == 0) TIMER0->TAILR = LOW;
		else if(x <= 1) {
			if((GPIOF->DATA & 8) ==8)TIMER0->TAILR = (20*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x <= 2) {
			if((GPIOF->DATA & 8) ==8)TIMER0->TAILR = (40*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x <= 3) {
			if((GPIOF->DATA & 8) ==8)TIMER0->TAILR = (60*LOW);
			else TIMER0->TAILR = LOW;
		}
		else{
			if((GPIOF->DATA & 8) ==8)TIMER0->TAILR = (80*LOW);
			else TIMER0->TAILR = LOW;
		}
		GPIOF->DATA  ^= 8;
	}
	
	else if(curr_temp>max){
		heating_pad_close();
		int x = abs(curr_temp-(max+5));
		GPIOF->DATA  &= 4;
		if(x == 0) TIMER0->TAILR = LOW;
		else if(x <= 1) {
			if((GPIOF->DATA & 4) ==4)TIMER0->TAILR = (20*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x <= 2) {
			if((GPIOF->DATA & 4) ==4)TIMER0->TAILR = (40*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else if(x <= 3) {
			if((GPIOF->DATA & 4) ==4)TIMER0->TAILR = (60*LOW);
			else TIMER0->TAILR = LOW;
		}
		
		else {
			if((GPIOF->DATA & 4) ==4)TIMER0->TAILR = (80*LOW);
			else TIMER0->TAILR = LOW;
		}
		GPIOF->DATA  ^= 4;
	}
	
	TIMER0->ICR |=0x01; //Clear the interrupt
	return;
}