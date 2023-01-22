#include "TM4C123GH6PM.h"

#define dig_T1 27504
#define dig_T2 26435
#define dig_T3 -1000

void sensor_init_config(){
			char write_arr[2];
			setSlave(0x76);	
			write_arr[0]=0xB6;	//reset sensor
			I2C0_Write_Multiple(0xE0,1,write_arr);
			write_arr[0]=0x23; //normal mode, x1 oversampling
			I2C0_Write_Multiple(0xF4,1,write_arr);
			write_arr[0]=0x00; //NO IIR filter is selected
			I2C0_Write_Multiple(0xF5,1,write_arr);
}

float raw_to_celsius(long adc_t){	//FROM THE DATASHEET OF BMP280
  float var1 = (((float)adc_t) / 16384.0 - ((float)dig_T1) / 1024.0) * ((float)dig_T2);
  float var2 = ((((float)adc_t) / 131072.0 - ((float)dig_T1) / 8192.0) *
                 (((float)adc_t) / 131072.0 - ((float)dig_T1) / 8192.0)) * ((float)dig_T3);
  return (var1 + var2) / 5120.0;
}
