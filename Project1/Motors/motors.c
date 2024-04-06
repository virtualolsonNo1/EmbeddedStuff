/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: motors.c
HW/Lab: Project 1

Purp: Implements a subset of motors for usage when driving motors on robot

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/

#include "motors.h"
#include "Arduino.h"

void motorSetup() {
  // set all IN pins on motor driver board as output
  DDRB |= (1 << IN1);
  DDRB |= (1 << IN2);
  DDRD |= (1 << IN3);
  DDRB |= (1 << IN4);

  // set motor initial directions
  PORTB &= ~(1 << IN1);
  PORTB |= (1 << IN2);

  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4);



  // set as output
  DDRD |= (1 << ENA);

  // set as output
  DDRB |= (1 << ENB);

  // Fast PWM with OCR1A as top
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);

  // Non-inverting mode for OCR1B and Prescaler 64
  TCCR2B = (1 << CS22);
}

void moveForward() {
  // move forwards
  // left motor forward
  PORTB |= (1 << IN1);    //set output low
  PORTB &= ~(1 << IN2);    //set output high

  // right motor forward
  PORTD &= ~(1 << IN3);    //set output low
  PORTB |= (1 << IN4);    //set output high


  OCR2B = 144;
  OCR2A = 144;
  delay(870);

  PORTB &= ~(1 << IN1);
  PORTB &= ~(1 << IN4);

  delay(500);

}

void moveBackward() {
  // move backwards
  // left motor backwards
  PORTB &= ~(1 << IN1);
  PORTB |= (1 << IN2);

  // right motor backwards
  PORTD |= (1 << IN3);
  PORTB &= ~(1 << IN4);

  // set duty cycle relative to 255
  OCR2B = 144;
  OCR2A = 144;
  delay(870);

  // reset motor pins that were high
  PORTB &= ~(1 << IN2);
  PORTD &= ~(1 << IN3);

  delay(500);

}

void turnLeft() {
  // turn left small
  // left motor backwards
  PORTB &= ~(1 << IN1);    //set output low
  PORTB |= (1 << IN2);    //set output high
  // right motor forwards
  PORTD &= ~(1 << IN3);    //set output low
  PORTB |= (1 << IN4);    //set output high


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN2);
  PORTB &= ~(1 << IN4);
  delay(500);

}

void turnRight() {
  // turn right small
  // left motor forwards
  PORTB |= (1 << IN1);    //set output low
  PORTB &= ~(1 << IN2);    //set output high
  // right motor backwards
  PORTD |= (1 << IN3);    //set output low
  PORTB &= ~(1 << IN4);    //set output high


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN1);
  PORTD &= ~(1 << IN3);
  delay(500);
}