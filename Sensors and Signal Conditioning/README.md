# Sensors and Signal Conditioning
In this lab we looked into translating signals from the analog world to the digit world. Sensors are necessary for accomplishing this.

## Sensors
Sensors are a way of converting physical data into an electrical value. For this lab, different sensor outputs need to be converted to a voltage, from which the value can be read through and Analog to Digital Converter (ADC) which the MSP430 Microprocessors have built in.

## Signal Conditioning
The signal conditioning was broken into multiple stages depending on the sensor used. First an output from the sensor may need to be amplified. An LM324 Quad OPAMP was used when the signal needed to be boosted. To reduce the noise that was also amplified, a lowpass filter may be needed before the signal is sent to the ADC.  


## ADC Resolution

The MSP430G2553 has a 10 Bit Resolution ADC with a maximum range of 0 to 3V. This results in 2.93mV per step of the ADC. More accurate results of the ADC occur when the input voltage is closer to the maximum value of 3V.  


## ADC Code


<s> Your code for this section should focus heavily on the ADC and being able to pull data from it. Your code need to communicate back to your computer using UART at 9600 Baud and send the ADC contents at 1 reading per second to start. This really is meant for you to see whether or not your signal conditioning is actually working and you can see changes in your sensor reading. This code should be very very similar to code you have written before and should be simple to port between your processors. </s>
  

## Hardware
 1. Temperature Sensor Circuit LM35 
 
 The LM35 temperature Sensor generates a voltage in reference to the enviornment temperature. According to the datasheet for the LM35, the sensor reads 10mV voltage for every degree celsius. Since the outside temperature was not expected to be over 100 degrees F, or 37.8 degrees C, the maximum input to the ADC was expected around .378mV. Since this value is no where near the 3V max the ADC can support, a non-inverting operational amplifier circuit with a gain of 7 was used. This brings the input to the ADC to be around 2.646V which still allows for some slightly higher temperatures to also work for the ADC's reading. Because a model for the temperature sensor was not available, the schematic for the voltage takes an input corresponding to the average temperature, around 22 degrees C, and amplifies that signal.
 
 A schematic design of the voltage that will enter the ADC can be seen below.
 
 ![screenshot_2](https://user-images.githubusercontent.com/31701000/32590953-10710e40-c4eb-11e7-9dcd-d40673a94b02.png)


Breadboard layout of the Temperature sensor circuit can be seen below.


![20171108_234110](https://user-images.githubusercontent.com/31701000/32590938-ff2eca82-c4ea-11e7-92ff-fd2f052a9f37.jpg)

 
 2. Photo Resistor Circuit
 
 A photo resistor is a device whose resistance varies due to the light influencing the resistor. As more light is applied to the resistor, the lower the resistance gets. In the same way, when in a darker area the resistance is signficantly higher. A slight video is included below, however it does not look as nice in the clip as it did was it was recorded. What should be seen is that when the flashlight of the phone is shown on the resistor, the LED blinks at a slower frequency.
 
 ![giphy-downsized-large](https://user-images.githubusercontent.com/31701000/32585812-490d0cd6-c4cd-11e7-9eff-325a8bda4d10.gif)
 
 
 
 3. Photo Transistor
 
<s> The hardware portion should be the same for each of the processors. You will need to have a total of 3 circuits made, each corresponding to a different sensor. You need to look at the range of measurements and the amount of resolution you can get out of your ADC and determine how to convert, scale, and filter your signal. Don't forget the fact that you will need to convert to a voltage, making some of these circuits not so trivial. The main goal as the hardware designer for these sensors is to provide the microprocessor with the best resolution to maximize the effectiveness of the ADC. <s/>

<s> ### README
The README for this part of the lab should talk briefly about how the ADC code works, but focus way more on the hardware aspect. You need to include schematics of your circuits, and well as supporting simulation/calculations which show that your circuits should work. You should talk about what types of circuits you are using and how they actually work. </s>
