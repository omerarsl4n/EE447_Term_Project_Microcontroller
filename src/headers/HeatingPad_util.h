#include "TM4C123GH6PM.h"

void init_gpio_heating_pad(){
	SYSCTL->RCGCGPIO |= 0x02; // turn on bus clock for GPIOF
	while((SYSCTL->PRGPIO & 0x0002) == 0){}; // READY ?
	GPIOB->DIR			|= 0x01; //set PB 0 as output 0001
  GPIOB->AFSEL		&= (0xFFFFFFFE);  // Regular port function
	GPIOB->AMSEL		=0; //Disable analog
	GPIOB->DEN			|=0x01; // Enable PB0 pin as a digital pin
	GPIOB->DIR       |= 0x01; //set as a digital output pin
}

void heating_pad_open(){	//PB0 for heating pad
	GPIOB->DATA |= 0x01;	//It should be connected to gate pin of the transistor
}

void heating_pad_close(){
	GPIOB->DATA &= ~(0x01);
}