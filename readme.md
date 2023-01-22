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
exceeds the range

# Hardware
**Prerequisites:**
- BMP280 Pressure and Temperature Sensor
- NOKIA 5110 LCD Screen
- Potentiometer
- RGB LED Placed on the TM4C123G Board
- Resistive Heating Pad

# Connections (Pinout)

It will be added soon.



