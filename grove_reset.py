#Turn off all LEDs after user shuts down system
import time
import grovepi

def Reset():
    # turn off all leds
    led1 = 3
    led2 = 4
    led3 = 5
    try:
        grovepi.pinMode(led1,"OUTPUT")
        grovepi.digitalWrite(led1, 0)           
        grovepi.pinMode(led2,"OUTPUT")
        grovepi.digitalWrite(led2, 0)
        grovepi.pinMode(led3,"OUTPUT")
        grovepi.digitalWrite(led3, 0)       
    except IOError:
        print ("Error")
Reset()
