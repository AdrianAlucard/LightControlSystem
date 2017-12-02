#Turn off all LEDs after user shuts down system
import time
import grovepi

def Reset():
    i=1
    while i<9:
        try:
          grovepi.pinMode(i,"OUTPUT")
          grovepi.digitalWrite(i, 1)           
          i=i+1
        except IOError:
            print ("Error")
Reset()
