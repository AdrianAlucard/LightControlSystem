#!/usr/bin/env python
#
# GrovePi Example for using the Grove Light Sensor and the LED together to turn the LED On and OFF if the background light is greater than a threshold.
# Modules:
# 	http://www.seeedstudio.com/wiki/Grove_-_Light_Sensor
# 	http://www.seeedstudio.com/wiki/Grove_-_LED_Socket_Kit
#
# The GrovePi connects the Raspberry Pi and Grove sensors.  You can learn more about GrovePi here:  http://www.dexterindustries.com/GrovePi
#
# Have a question about this example?  Ask on the forums here:  http://forum.dexterindustries.com/c/grovepi
#
'''
## License

The MIT License (MIT)

GrovePi for the Raspberry Pi: an open source platform for connecting Grove Sensors to the Raspberry Pi.
Copyright (C) 2017  Dexter Industries

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
'''
import signal
import sys

import time
import grovepi
#can only use grovepi.analogWrite(led, val) on ports 3,5,6,9
def signal_handler(signal, frame):
        print('Automode Complete')
        sys.exit(0)
        
def LightSensor():
    # Connect the Grove Light Sensor to analog port A0
    # SIG,NC,VCC,GND
    light_sensor = 0

    # Connect the LED to digital port D4
    # SIG,NC,VCC,GND
    led = 5
    led2 = 3

    # Turn on LED once sensor exceeds threshold resistance
    threshold = 10

    grovepi.pinMode(light_sensor,"INPUT")
    grovepi.pinMode(led,"OUTPUT")
    grovepi.pinMode(led2,"OUTPUT")
    
    while True:
        try:
            # Get sensor value
            sensor_value = grovepi.analogRead(light_sensor)

            # Calculate resistance of sensor in K
            resistance = (float)(1023 - sensor_value) * 10 / sensor_value
            brightness = 1023-sensor_value
            #gradually adjust brightness
            
            grovepi.analogWrite(led, brightness)
            grovepi.analogWrite(led2, brightness)
            
          #  if resistance > threshold:
                # Send HIGH to switch on LED
          #      grovepi.digitalWrite(led,1)
       #     else:
                # Send LOW to switch off LED
           #     grovepi.digitalWrite(led,0)

            print("sensor_value = %d resistance =%.2f brightness =%d" %(sensor_value,  resistance, brightness))
            time.sleep(.5)

        except (IOError):
            print ("Error")

        except (KeyboardInterrupt):
            signal.signal(signal.SIGINT, signal_handler)
            
LightSensor()

#signal.signal(signal.SIGINT, signal_handler)
#print('Press Ctrl+C')
#signal.pause()
