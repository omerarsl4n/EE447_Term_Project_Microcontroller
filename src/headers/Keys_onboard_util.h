#include "TM4C123GH6PM.h"

extern float min;
extern float max;

void init_push_buttons(){
		SYSCTL->RCGCGPIO |= 0x20; // turn on bus clock for GPIOF
		while((SYSCTL->PRGPIO & 0x0020) == 0){}; // READY ?
		GPIOF->LOCK = 0x4C4F434B; //UNLOCK
		GPIOF->CR = 0x01; // Enable GPIOPUR register enable to commit
		GPIOF->LOCK = 0; 
		GPIOF->PUR |= 0x11;			
		GPIOF->DIR	&= ~(0x11); //set PF4-PF0 as input
		GPIOF->DEN |= 0x11;
		GPIOF->IS &= ~(0X11); //edge-sensetive
		GPIOF->IBE &= ~(0X11); //not both-edge
	  GPIOF->IEV &= ~(0X11); //falling edge
		GPIOF->IM  |= (0X11);
		GPIOF->ICR |= (0X11);
		NVIC->IP[30] = 3 << 5;     // set interrupt priority to 3 
		NVIC->ISER[0] = (1 << 30); // ENABLE interrupt
			
}	

void GPIOF_Handler(void){		
	
  if (GPIOF->MIS & 0x10) // check if interrupt causes by PF4/SW1 which minset
   {   
			min = get_temp();
   } 
   else if (GPIOF->MIS & 0x01) // check if interrupt causes by PF0/SW2 whic maxset
    {   
			max = get_temp();
    }
	GPIOF->ICR |= 0x11; /* clear the interrupt flag */
}
