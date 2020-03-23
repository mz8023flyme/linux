import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM) 
GPIO.setup(27,GPIO.OUT)
GPIO.setwarnings(False)

try:
    while True:
        GPIO.output(27,GPIO.HIGH)
        time.sleep(1)
        GPIO.output(27,GPIO.LOW)
        time.sleep(1)
except:

     print("except")
