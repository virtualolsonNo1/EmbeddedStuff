/*--------------------------------------------------------------------
Name:   Carl Olson
Date:   April 10th, 2024 
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   main.cpp
HW/Lab: Lab 4, Button Interrupt

Purp: Button Interrupt code for Lab 4 to introduce students to interrupts and ISRs

Doc:  NA

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>

#define BUTTON 3
#define LED 4

void setup() {
  //Set pinmode for Button as input
    DDRD &= ~(1 << BUTTON);
  //set output to enable pullup resistor
    PORTD |= (1 << BUTTON);

  // Set pinmode for LED as output
    DDRD |= 1 << LED;

    // enable interrupt on falling edge
    EICRA = 0x8;
    // enable int1 interrupt generation
    EIMSK = 0x2;
    // clear interrupt flags
    EIFR = 0x0;

    Serial.begin(9600);
    Serial.println("Setup Complete!!!");

}

ISR(INT1_vect){
//INT1 interrupt handling code goes here
  PORTD |= (1 << LED);
}

void buttonPress() {
  // if the button is high (i.e. not pressed), enter if statement and turn off LED
  if (!(PIND & (1 << BUTTON))) {
  Serial.println("LED ON!!!");
  PORTD |= (1 << LED);
  } else {
    PORTD &= ~(1 << LED);
    Serial.println("LED OFF!!!");
  }
}

void buttonPressInterruptNotWorkingCorrect() {
  // if the button is high (i.e. not pressed), enter if statement and turn off LED
  if (PIND & (1 << BUTTON)) {
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    PORTD &= ~(1 << LED);
  }
  
}

void solution1() {
  // if the button is high (i.e. not pressed), enter if statement and turn off LED
  if (PIND & (1 << BUTTON)) {
    // disable interrupts
    cli();
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    PORTD &= ~(1 << LED);
    // enable interrupts
    sei();
  }
  
}

void solution2() {
  // set EICRA to 0 to trigger when pin goes low
  EICRA = 0x0;
  // if the button is high (i.e. not pressed), enter if statement and turn off LED
  if (PIND & (1 << BUTTON)) {
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    PORTD &= ~(1 << LED);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  // buttonPress();
  // solution1();
  solution2();
}

