# Sensors and Signal Conditioning
In this lab we looked into translating signals from the analog world to the digit world. Sensors are necessary for accomplishing this.

## Sensors
Sensors are a way of converting physical data into an electrical value. For this lab, different sensor outputs need to be converted to a voltage, from which the value can be read through and Analog to Digital Converter (ADC) which the MSP430 Microprocessors have built in.

## Signal Conditioning
The signal conditioning was broken into multiple stages depending on the sensor used. First an output from the sensor may need to be amplified. An LM324 Quad OPAMP was used when the signal needed to be boosted. To reduce the noise that was also amplified, a lowpass filter may be needed before the signal is sent to the ADC.  

## ADC Resolution

The MSP430G2553 has a 10 Bit Resolution ADC with a maximum range of 0 to 3V. This results in 2.93mV per step of the ADC. More accurate results of the ADC occur when the input voltage is closer to the maximum value of 3V.  

## ADC Code
The ADC code used is the same for all three sensors used for the lab. The way the code works is that the program simply takes the value stored in the ADC10MEM register and assigns it to a variable called 'temp'. Upon sending the character 'A' to the processor via UART, the user would receive back two hexadecimal numbers, the first representing the most significant bit stored in memory and the second representing the least significant bit. As mentioned previously due to the simplicity of the code it can be used to display the ADC value of any of the three sensors used in the lab. This means that it is up to the user to calculate the brightness in lumens or the temperature in celcius or fahrenheit in order to interpret the readings. A future version of this program would include calculations that are built into the code to generate values that the user could immediately interpret. 
  
## Hardware
 1. Temperature Sensor Circuit LM35 
 
 The LM35 temperature Sensor generates a voltage in reference to the enviornment temperature. According to the datasheet for the LM35, the sensor reads 10mV voltage for every degree celsius. Since the outside temperature was not expected to be over 100 degrees F, or 37.8 degrees C, the maximum input to the ADC was expected around 378mV. Since this value is no where near the 3V max the ADC can support, a non-inverting operational amplifier circuit with a gain of 7 was used. This brings the input to the ADC to be around 2.646V which still allows for some slightly higher temperatures to also work for the ADC's reading. Because a model for the temperature sensor was not available, the schematic for the voltage takes an input corresponding to the average temperature, around 22 degrees C, and amplifies that signal.
 
 A schematic design of the voltage that will enter the ADC can be seen below.
 
 ![screenshot_2](https://user-images.githubusercontent.com/31701000/32590953-10710e40-c4eb-11e7-9dcd-d40673a94b02.png)


Breadboard layout of the Temperature sensor circuit can be seen below.


![20171108_234110](https://user-images.githubusercontent.com/31701000/32590938-ff2eca82-c4ea-11e7-92ff-fd2f052a9f37.jpg)

 
 2. Photo Resistor Circuit
 
 A photo resistor is a device whose resistance varies due to the light influencing the resistor. As more light is applied to the resistor, the lower the resistance gets. In the same way, when in a darker area the resistance is signficantly higher. In order to capture the data from this type of device in which resistance varies, the best approach is to use a voltage divider network, where the first resistor R1 is the photoresistor and the second resistor R2 is a 10 kΩ resistor. The image below shows that as the resistance of the photoresistor increases from 1 kΩ to 10 kΩ, the voltage supplied to the ADC drops considerably and as a result the brightness stored in memory is reduced.
 
![photoresistor](https://user-images.githubusercontent.com/31484781/32639649-e2e1af72-c592-11e7-9f83-cf96d51eab61.JPG)

In the next image is the same circuit seen above on the breadboard.

![2017-11-09 20 05 32](https://user-images.githubusercontent.com/31484781/32639679-01ffc786-c593-11e7-9a37-e48f15e8c999.jpg)

Next, the values from the ADC10 for this circuit will be shown via REALTERM first when the lights are on, then when they are turned off.

![realterm lights on](https://user-images.githubusercontent.com/31484781/32639653-e921f8a6-c592-11e7-9ee3-2015387da8aa.JPG)

![lights off](https://user-images.githubusercontent.com/31484781/32639659-ec10f92c-c592-11e7-84cb-a2f113e83118.JPG)

As shown, the ADC values drop considerably when the lights are turned off, which confirms the notion that as the resistance increases in the photoresistor, the voltage supplied to the ADC drops.
 
 A video of the same circuit powering an RGB LED is included below, however it does not look as nice in the clip as it did was it was recorded. What should be seen is that when the flashlight of the phone is shown on the resistor, the LED blinks at a slower frequency.
 
 ![giphy-downsized-large](https://user-images.githubusercontent.com/31701000/32585812-490d0cd6-c4cd-11e7-9eff-325a8bda4d10.gif)


