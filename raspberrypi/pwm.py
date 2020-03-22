#-*- coding=UTF-8 -*-
import RPi.GPIO as GPIO
import time
LED = 17
GPIO.setmode(GPIO.BCM) 
GPIO.setup(LED,GPIO.OUT)
GPIO.setwarnings(False)
p = GPIO.PWM(LED,100) #频率
p.start(0) #占空比
try:
    while True:
        for i in range(100):
            p.ChangeDutyCycle(i)
            time.sleep(0.01)
        time.sleep(0.2)
        for i in range(100):
            p.ChangeDutyCycle(100 - i)
            time.sleep(0.01)
        time.sleep(0.2)
except:
     print("except")
p.stop()
GPIO.cleanup()
