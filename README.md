# Arduino RGB LED Controller

This Arduino script uses a potentiometer and pushbutton to control RGB LEDs. I created this for a DIY project to install some lights into a figurine shelf and have a cool glowing effect.

# Setup
1. Wire up your arduino. The script is setup for an arduino uno: 
    - Pushbutton to pin 2
    - Potentiometer to pin A0
    - R,G,B to pins 11,10,9 respectively
2. Upload script to arduino

# How to Use

The pushbutton allows for three functions to be performed:
| Press Length | Function |
| ------ | ------ |
| < 1 second | Change color (r,g,b) to control |
| >= 1 second  AND  < 2 seconds | Toggle LEDs on/off  |
| >= 2 seconds | Toggle pattern on/off |

 - When you press the pushbutton for less than a second it will cycle through R,G,B so you can control the color ouput with the potentiometer.
Note: When the arduino is turned on, the LEDs will initialize to a random color and the color being controlled by the potentiometer is initialized to red.
 - Toggling the pattern will produce a fade effect color to color based on the defined pattern in the script. The default pattern I've uploaded is the colors of the rainbow. 



