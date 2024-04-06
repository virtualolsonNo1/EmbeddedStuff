/*--------------------------------------------------------------------
Name: 	Carl Olson
Date: 	02/05/2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: 	main.cpp
HW/Lab:	Lab 1, Question 4.1

Purp:	Learn about photoresistors by finding the right resistor value to maximize the range of the photoresistor's output between 0 and 1023


Doc:	No one helped me

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>

#define PHOTORES A0

//variable to store output value from photoresistor
int val = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  //initialize pin connected to photoresistor as input
  pinMode(PHOTORES, INPUT);

  //start serial so can view serial output in terminal
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  // read in value from photoresistor
  val = analogRead(PHOTORES);

  // print analog value
  Serial.println(val);
  
}
