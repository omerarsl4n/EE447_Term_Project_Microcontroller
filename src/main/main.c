#include "TM4C123GH6PM.h"
#include "I2C_util.h"
#include "BMP280_util.h"
#include "HeatingPad_util.h"
#include "ADC0_util.h"
#include "LED_onboard_util.h"
#include "Keys_onboard_util.h"
#include "helpers.h"

uint16_t temperature_arr[256];
float min,max,curr_temp;

extern void InitSPI();
extern void initializeScreen();
extern void drawMeasure();
extern void	drawMin();
extern void drawMax();

int main(){
		
		char read_arr[2];
	  long res;
	  int i =0,error;
		//Initilization of required peripherals
		init_gpio_PB();//I2C PINS CONFIG
	  init_i2c_PB();	
		init_led();
		init_adc_PE();
		init_push_buttons();
		sensor_init_config();
		init_gpio_heating_pad();
		InitSPI();
		initializeScreen();
	
		while(1){		
			res = 0;
			for(i=0;i<256;i++){	//start taking 256 samples of temperature data
				error = I2C0_read_Multiple(0xFA,2,read_arr);
				if(error){
					i--;
					continue;
				}
				res += (read_arr[0] << 8) + read_arr[1];	//read 16 bit data from sensor and sum it for averaging at the end
				temperature_arr[i]=res;
				delay(1000);//delay to wait 10 ms
			}
			res = res / 256; 	//find average of 256 samples of temp data
			curr_temp = raw_to_celsius(res<<4); //4 bit shifting for proper conversion, the result stored in 16 bit actually
			temp_to_screen(curr_temp,0);	//0 means current temperature is transmitted
			temp_to_screen(min,1);				//1 means min threshold temperature is transmitted
			temp_to_screen(max,2);				//2 means max threshold is transmitted
			drawMeasure();			//Plot new values if needed
		  drawMin();
		  drawMax();
		}
		
}
