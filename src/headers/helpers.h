#include "TM4C123GH6PM.h"

volatile int *tempval  = (volatile int*)0x20007000;	//storing address for current temperature 
volatile int *minval  =  (volatile int*)0x20007004; //storing address for min temperature 
volatile int *maxval  =  (volatile int*)0x20007008; //storing address for max temperature 

void delay(int x){
		int i;	
		for(i=0;i<x;i++); //for 100 ms delay x should be 10^5
}


void temp_to_screen(float x, char a){	//a=0 currtemp, a=1 min, a=2 max 
		int temp;												//This function converts the float data to readible version for Assembly
	  int hold;												//Assembly code reads data from the addresses and renew screen with new ones.
		temp = x /10;
		if(a==0){
			hold = (temp%10);
			temp = x;
			hold += ((temp%10)<<8);
			temp = x * 10;
			hold += ((temp%10)<<16);
			*tempval =hold;
		}
		
		else if(a==1){
			hold = (temp%10);
			temp = x;
			hold += ((temp%10)<<8);
			temp = x * 10;
			hold += ((temp%10)<<16);
			*minval =hold;
		}
		else{
			hold = (temp%10);
			temp = x;
			hold += ((temp%10)<<8);
			temp = x * 10;
			hold += ((temp%10)<<16);
			*maxval =hold;
		}
		
}

