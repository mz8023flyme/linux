/*************************************************************************
	> File Name: led.c
	> Author: life
	> Created Time: Sun 22 Mar 2020 15:03:49 GMT
 ************************************************************************/

#include<stdio.h>
#include <wiringPi.h>

int main(void) {

   wiringPiSetup();

   pinMode (13, OUTPUT);

   for(;;) {

       digitalWrite(13, HIGH);delay (500);

       digitalWrite(13, LOW);delay (500) ;

    }

}
