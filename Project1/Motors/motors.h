/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: motors.h
HW/Lab: Project 1

Purp: Implements a subset of motors for defines and function declarations

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/

// preprocessor directives
#define ENA PIND3
#define IN1 PB1
#define IN2 PB4

#define IN3 PIND2
#define IN4 PINB5
#define ENB PINB3

// function declarations
void motorSetup();
void moveForward();
void moveBackward();
void moveLeft();
void moveRight();