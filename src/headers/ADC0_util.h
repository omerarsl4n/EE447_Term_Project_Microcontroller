#include "TM4C123GH6PM.h"

void init_adc_PE(){
	SYSCTL->RCGCGPIO |= 0x10; // ENABLE PORT E CLOCK 
	while((SYSCTL->PRGPIO & 0x0010) == 0){}; // READY ?
	GPIOE->DIR &= ~(0x08);
	GPIOE->AFSEL |= 0x08;
	GPIOE->DEN &= ~(0x08);
	GPIOE->AMSEL |= (0X08);
		
	SYSCTL->RCGCADC |= 0x01;
	while((SYSCTL->PRADC & 0x0001) == 0){}; // READY ?
	ADC0->ACTSS &= ~(0x08);	// Disable SS
	ADC0->EMUX &= ~(0XF000);  //Select trigger
	ADC0->SSMUX3 &= ~(0X0F);  // Select input pin AIN0
	ADC0->SSCTL3 |= (0X06);	// set final and interrupt enable
	ADC0->PC  = 0X1; // input rate 125kbps
	ADC0->ACTSS |= (0x08);	// Enable SS
	ADC0->IM &= ~(0X08); //disable interrugt for NVIC
}


float get_temp(){ //Take temperature from analog channel, it is connectod POT actually.
	uint16_t take;
	float result = 20.0;
	ADC0->PSSI |= (0X08);	//initiate from software
	while((ADC0->RIS & 0x8)==0) {;} // polling
	take = (ADC0->SSFIFO3)/40;		//when data is available, read
	ADC0->ISC |= (0X08);		//clear the flag !!!
	result += (take*0.1);
	return result;
}