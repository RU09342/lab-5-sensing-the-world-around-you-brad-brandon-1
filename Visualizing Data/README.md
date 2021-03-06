# Visualizing Data
In the next lab, we will start looking into how to take action on the information your microprocessor is receiving, for the last part of this lab, we will focus on visualizing the data. We will focus on three main methods of visualization. For each of these methods, you need to pick between 1-3 processors based on the part. As with Milestone 1, you will need to talk in this README about why you picked the processors you did (for one part, it is going to be painfully obvious). Overall, you should aim to use all five processors by the end of this part of the lab, however _YOU DO NOT NEED TO USE ALL FIVE FOR EACH PART_.

## RGB LED
Dependencies: The RGB LED code is dependent only on msp430.h

The file MSP430G2553 LED_BLINK_PHOTORESISTOR contains code to control an RGB LED using the ADC10 on the MSP430G2553. The ADC in this case takes in the output of the photoresistor circuit with the corresponding ADC pin being P1.3. The code then uses a very simple if statement in the main function to control the rate at which the red LED (P1.0) on the RGB LED blinks. If the value of light detected in the ADC10 memory register is less than a certain hexadecimal threshold, the LED will blink at a fast rate, but when exposed to brighter light at a higher hexadecimal value, the blinking slows down considerably. This is shown in the following video:
 
## LCD Display
Dependencies: This code is dependent upon two header files: LCDDriver.h and msp430.h

The LCD Display code works by reading and displaying the value of the ADC12MEM0 register every time a user sends the character 'A' over UART. By shifting and then truncating the 12 bit value of ADC12MEM0, one hexadecimal character corresponding to four binary bits can be represented on each of four digits on the LCD display. Additional code was added to handle cases where the hexadecimal value to be displayed was a letter since without this additional code the program tried to represent numbers such as 10 on a single digit in the LCD display. The first two digits of the display are automatically set to '0' and 'x' so that the user can see the hexadecimal value in memory show up in the right format on the LCD monitor. The code is verified to be working by also reading the memory over UART. Images showing the code reading from the ADC12MEM0 register over UART and on the LCD display are shown below. When viewing the UART image, the four hexadecimal values shown in gold each correspond to one four-bit byte of information which will be shown on the LCD monitor. Keep in mind that the values shown in UART are truncated on the LCD to only include the right-most character in each hexadecimal number. 

![realterm lcd](https://user-images.githubusercontent.com/31484781/32640018-91e4f8c0-c594-11e7-802d-93303ca495ed.JPG)

![](https://user-images.githubusercontent.com/31484781/32639684-091d02c2-c593-11e7-952a-3d42663b25e4.jpg)

