#-*- coding=UTF-8 -*-
import RPi.GPIO as GPIO
import time
key = 17
led = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(key,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(led,GPIO.OUT)
GPIO.setwarnings(False)



try:
   # 轮询
    while True:
        if (GPIO.input(key)):
            GPIO.output(led,GPIO.LOW)
            #print('按下')
        else :
            GPIO.output(led,GPIO.HIGH)
            #print('松开')
except:
    print("execpt")



