/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: ultrasonic.cpp
HW/Lab: Lab 3

Purp: Implements a subset ultrasonic and servo setup/usage

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/

#include <Arduino.h>
#include "ultrasonic.h"


void servoSetup() {
  // set as output
  DDRB |= (1 << OCR1B_PWM);

  // Fast PWM with OCR1A as top
  TCCR1A = (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);

  // Non-inverting mode for OCR1B and Prescaler 1024
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TCCR1C = 0;
  TCNT1 = 0;

  // this give us a 20ms pulse width, as .02 / (1 / 16000000)
  OCR1A = 313;
  // To move servo left, 1ms, so // this give us a 20ms pulse width, as .001 / (1 / 16000000)
  OCR1B = 15;
}

void ultrasonicSetup() {
  // Set pinmode for trigger pin to output 
  DDRD |= (1 << TRIGGER_PIN);

  //Init counter1
  //Normal mode 0xffff top, rolls over
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));

  // set prescaler to 1024
  TCCR1B &= ~(1 << CS11);
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // set rising or falling edge, in this case falling
  TCCR1B &= ~(1 << ICES1);
  TCCR1C = 0;

  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  //Make sure interrupts are disabled 
  TIMSK1 = 0;
  TIFR1 = 0;
}

void ultrasonicReading(int direction) {
  // set trigger pin high for 10us 
  PORTD |= (1 << TRIGGER_PIN);
  delayMicroseconds(10);
  PORTD &= ~(1 << TRIGGER_PIN);

  // wait until input capture pin goes high
  while(!(PINB & (1 << ICP1_PIN))) {}

  // clear ICR1 and TCNT1
  ICR1 = 0;
  TCNT1 = 0;

  // delay for 60ms measurement cycle
  delay(60);

  // read in ICR1 value
  float value = ICR1;

  // if ICR1 value converted to inches is less than 12, turn on the correct LED(s), otherwise, simply print that value
  if (value * 64 / 148 < 12) {
    Serial.println("Wall closer than 12 inches!!!");
    Serial.println(value * 64 / 148);

  } else {
    Serial.println(value * 64 / 148);
  }
}

void moveRight() {
  // set up timer for PWM servo movement
  servoSetup();

  // Move servo right
  OCR1B = 12;
  Serial.println("Move Servo right");
  delay(1000);
}

void moveMiddle() {
  // set up timer for PWM servo movement
  servoSetup();

  // Move servo middle
  OCR1B = 26;
  Serial.println("Move Servo middle");
  delay(1000);
}

void moveLeft() {
  // set up timer for PWM servo movement
  servoSetup();

  // Move servo left
  OCR1B = 40;
  Serial.println("Move Servo left");
  delay(1000);

}


// example code for combining servo with ultrasonic sensor using same timer (timer1)
void servoAndUltrasonicExample() {
  moveRight();

  // setup timer for ultrasonic sensor and take reading
  ultrasonicSetup();

  ultrasonicReading(0);

  moveMiddle();

  // setup timer for ultrasonic sensor and take reading
  ultrasonicSetup();
  ultrasonicReading(1);

  moveLeft();

  // setup timer for ultrasonic sensor and take reading
  ultrasonicSetup();
  ultrasonicReading(2);
  
}
