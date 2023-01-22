# Temperature Controller Project

# Description
In this project, you are expected to build a temperature controller system based on an obtained temperature signal. You are going to detect temperature samples using a temperature sensor, and try to
keep the temperature inside a cardboard box constant using resistive heating pad. To show the current
configuration and measurements, you will use a Nokia 5110 LCD, which you will drive using SPI module.
You will also use GPIO for the on-board RGB LED and pushbuttons.

# Temperature Sensing
The system continuously senses the temperature inside a cardboard box using a BMP280 pressure and
temperature sensor. It stores samples in an array of 256 elements. When the array is filled, your system
calculates the average temperature and updates the output elements’ states.

# User Interface
The system has three output elements. Firstly, on the LCD screen, detected temperature and the
temperature range are displayed. Secondly, on-board LEDs are turned on or off according to the detected
temperature. If the temperature is below the range, red LED must be on and the others must be off.
If the temperature is in the range, green LED is on. And blue LED must be on, when the temperature
exceeds the range. When a LED is on, its brightness can change proportional to the current
temperature. That is the LED should lights up less in low temperature and more in high
temperature.


# Hardware
**Prerequisites:**
- BMP280 Pressure and Temperature Sensor
- NOKIA 5110 LCD Screen
- Potentiometer
- RGB LED Placed on the TM4C123G Board
- Resistive Heating Pad

# Connections (Pinout)

**SPI PINS:**
- PA2: SSI0Clk
- PA3: SSI0Fss
- PA5: SSI0Tx

**I2C PINS:**
- PB2: I2C0SCL
- PB3: I2C0SDA

**TIMER PINS:**
- PB6: T0CCP0

**ADC PINS:**
- PE3: AIN0

**Heating Pad Driver**
- PB0: Applied to gate pin of the MOSFET



**LEDs and Push Buttons**
- PF0: Min Set (SW1)
- PF1: Red Light
- PF2: Blue Light
- PF3: Green Light
- PF4: Max Set (SW2)





