/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: ultrasonic.h (ultrasonic header)
HW/Lab: Lab 3

Purp: Implements a subset ultrasonic for defines and function declarations

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/

#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

// includes
#include <Arduino.h>

// preprocessor directives/macros
#define ONBOARD_LED PINB5
#define RED_LED PIND5
#define GREEN_LED PIND6
#define BLUE_LED PIND7
#define OCR1B_PWM PINB2
#define ICP1_PIN PINB0
#define TRIGGER_PIN PIND4

// function declarations
void servoSetup();
void ultrasonicSetup();
void ultrasonicReading(int direction);
void moveRight();
void moveMiddle();
void moveLeft();

#endif