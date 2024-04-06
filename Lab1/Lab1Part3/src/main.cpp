/*--------------------------------------------------------------------
Name: 	Carl Olson
Date: 	02/05/2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: 	main.cpp
HW/Lab:	Lab 1, Question 3

Purp:	Learn about reading a schematic via connecting and programming an RGB LED

Doc:	No one helped me

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>

#define RED PD5
#define GREEN PD6
#define BLUE PD7
#define BUTTONPIN A5

//variable to store if button is high or low
int buttonState = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize LED pins as outputs
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  //initialize button pin as an input
  pinMode(BUTTONPIN, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  // read in value of button
  buttonState = digitalRead(BUTTONPIN);

  // if the button is pressed and goes low, light up LED in following pattern
  if(buttonState == LOW) {
    //R
    digitalWrite(RED, HIGH);
    delay(1000);
    //RG
    digitalWrite(GREEN, HIGH);
    delay(1000);
    //G
    digitalWrite(RED, LOW);
    delay(1000);
    //GB
    digitalWrite(BLUE, HIGH);
    delay(1000);
    //B
    digitalWrite(GREEN, LOW);
    delay(1000);
    //RB
    digitalWrite(RED, HIGH);
    delay(1000);
    //Turn off colors that were on
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, LOW);
  }
  
}
