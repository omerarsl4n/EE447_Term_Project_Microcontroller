#include "TM4C123GH6PM.h"

// This header file includes all functions about I2C like initialization of ports, read ops, write ops, etc.

void init_gpio_PB(){
	SYSCTL->RCGCGPIO |= 0x02; // ENABLE PORT B CLOCK 
	while((SYSCTL->PRGPIO & 0x0002) == 0){}; // READY ?
	GPIOB->DEN |= (0X0C);		//Digital enable PB2-3
	GPIOB->AFSEL |= 0x0C;		//Open alternate func
	GPIOB->PCTL |= 0x00003300; // choose I2C
	GPIOB->ODR  |= 0X08; //Open drain for SDA PB3
	GPIOB->PUR  |= 0X0C; //Pull up for SDA and SCL
}

void init_i2c_PB(){
	SYSCTL->RCGCI2C |= 0X01;  //ENABLE I2C 0
	while((SYSCTL->PRI2C & 0x0001) == 0){}; // READY ?
	I2C0->MCR |= 0X10; // Set as Master
	I2C0->MTPR |= 0x07; // 100Kbps when sysclock 16 MHz
}

static int I2C_wait_till_done(void)
{
    while(I2C0->MCS & 1);   // wait until I2C master is not busy 
    return I2C0->MCS & 0xE; // return I2C error code, 0 if no error
}

void setSlave(uint8_t slaveAddress){
		I2C0->MSA = (slaveAddress<<1);
}

void setRW(uint8_t mode){
	if(mode) I2C0->MSA |= 0x01;	
	else I2C0->MSA &= ~(0x01);
}

char I2C0_Write_Multiple(char slave_memory_address, int bytes_count, char* data){   
    char error;
    if (bytes_count <= 0)return -1;       // there is no write operation
		setRW(0);	//enable writing mode
    I2C0->MDR = slave_memory_address;
    I2C0->MCS = 3;                  //Start and Run

    error = I2C_wait_till_done();   // wait until address is transmitted
    if (error) return error;

    // start sending datas
    while (bytes_count > 1)
    {
        I2C0->MDR = *data++;             //write the first byte
        I2C0->MCS = 1;                   //Run and no stop
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
    }
    
    //send last byte
    I2C0->MDR = *data++;                 // write the last byte 
    I2C0->MCS = 5;                       // send stop flag with run
    error = I2C_wait_till_done();
    while(I2C0->MCS & 0x40);             // POLL BUSBSY BIT
    if (error) return error;
    return 0;       // writing operation is succesful.
}


char I2C0_read_Multiple(char slave_memory_address, int bytes_count, char* data){
    char error;	

    if (bytes_count <= 0) return -1;         // there is no read operation
		setRW(0); //set writing mode first
    I2C0->MDR = slave_memory_address;
    I2C0->MCS = 3;       //Start and Run
    error = I2C_wait_till_done();
    if (error) return error;
    
		setRW(1);//set reading mode
		
		if (bytes_count == 1) I2C0->MCS = 7;       //last byte condition
    else  I2C0->MCS = 0xB;            //ack and start, no stop cond
		
    error = I2C_wait_till_done();
		
    if (error) return error;

    *data++ = I2C0->MDR;            // store the data 

    if (--bytes_count == 0){
        while(I2C0->MCS & 0x40);    // POLL BUSBSY BIT
        return 0;       
    }
 
    // Continue reading
    while (bytes_count > 1)
    {
        I2C0->MCS = 9;              //ACK and run
        error = I2C_wait_till_done();
        if (error) return error;
        bytes_count--;
        *data++ = I2C0->MDR;        // store data
    }

    I2C0->MCS = 5;                  //last reading so send stop bit
    error = I2C_wait_till_done();
    *data = I2C0->MDR;              // store the last data
    while(I2C0->MCS & 0x40);        // Poll BUSBSY BiT 
    
    return 0;       // Operation end successfuly
}
