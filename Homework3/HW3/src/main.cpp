/*--------------------------------------------------------------------
Name:   Carl Olson
Date:   February 28th, 2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   main.cpp
HW/Lab: Homework 3

Purp: Further introduce students to peripheral configuration using registers by giving them a project of using a pushbutton and RGB LED to implement morse code

Doc:  N/A

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/


#include <Arduino.h>
#include "morse.h"

// result values for part 2 of homework
volatile int8_t result_8;
volatile int16_t result_16;
volatile int32_t result_32;
volatile float result_f;

// bool to only allow entrance to section of code on first runthrough
volatile bool firstTime = true;

// bool to only allow the long pause symbol (s) to be printed once
volatile bool printedPause = true;

// store time for keeping track of if long pause occurrs
volatile int time = 0;

// buffer to store last 4 morse code values input through the button
volatile uint8_t last4Morse[4];

//red, green, and blue morse code values for r, g, and b respectively
const uint8_t redMorse[3] = {0, 1, 0};
const uint8_t greenMorse[3] = {1, 1, 0};
const uint8_t blueMorse[4] = {1, 0, 0, 0};

// Definitions for LED assignments:
#define BOARD_LED 5   //pin 13 is PortB bit 5
#define RED_LED 1     //pin 9 is PortB bit 1  
#define GREEN_LED 2  //pin 10 is PortB bit 2
#define BLUE_LED 3   //pin 11 is PortB bit 3
#define BUTTON 2



void problem2() {
  // put your main code here, to run repeatedly:
  if(firstTime) {
    firstTime = false;
    int8_t a,b;
    int16_t c,d;
    int32_t e,f;
    float g,h;

    a = (int8_t) (rand() & 0xFF);
    b = (int8_t) (rand() & 0xFF);

    result_8 = a-b;
    asm volatile ("nop");
    result_8 = a/b;


    c = (int16_t) (rand() & 0xFFFF);
    d = (int16_t) (rand() & 0xFFFF);

    result_16 = c-d;
    asm volatile ("nop");
    result_16 = c/d;

    e = (int32_t) (rand() & 0xFFFFFFFF);
    f = (int32_t) (rand() & 0xFFFFFFFF);

    result_32 = e-f;
    asm volatile ("nop");
    result_32 = e/f;


    g = (float) (rand() & 0xFFFF) / 10;
    h = (float) (rand() & 0xFFFF) / 10;
    int x = micros();
    result_f = g - h;
    int y = micros();
    Serial.print("Subtraction microseconds: ");
    Serial.println(y - x);
    asm volatile ("nop");
    x = micros();
    result_f = g / h;
    y = micros();
    Serial.print("Division microseconds: ");
    Serial.println(y - x);

  }

}

void problem4Setup() {
  //Set pinmode for Button as input
  DDRC &= ~(1 << BUTTON);
  //Enable pullup 
  PORTC |= (1 << BUTTON);  //set output to enable pullup resistor
  // clarify PUD is 0
  MCUCR &= ~(1 << PUD);


  //arduino code to turn on internal pullup: 
  // pinMode(PC2, INPUT_PULLUP);
}

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
 //Set pinmode for LEDs to output 
  DDRB |= (1 << BOARD_LED);
  DDRB |= (1 << RED_LED);
  DDRB |= (1 << GREEN_LED);
  DDRB |= (1 << BLUE_LED);

  //Turn all off
  PORTB &= ~(1 << BOARD_LED); //clear output
  PORTB &= ~(1 << RED_LED);   //clear output
  PORTB &= ~(1 << GREEN_LED); //clear output
  PORTB &= ~(1 << BLUE_LED);  //clear output
  }

void problem4() {
  // if button is not pressed, enter, otherwise, if button is low, enter else statement
  if (PINC & (1 << BUTTON)) {
    Serial.println("No Button Press");
  } else {
    Serial.println("Button pressed");
  }
 
}

void problem5c() {
  // print Hi once upon startup
  if (firstTime) {
    firstTime = false;
    char H = 'H';
    char i = 'i';
    morseBlinkChar(2, H);
    morseShortPause();
    morseBlinkChar(1, i);
  }
}

void problem5d() {
  // if button is pressed, time button press
  if (!(PINC & (1 << BUTTON))) {
    time = millis();
    while(!(PINC & (1 << BUTTON))) {}
    time = millis() - time;
    printedPause = false;

    delay(100);
    if (time < 10) {
      return;
    }

    // if button pressed less than 1 sec, print d, otherwise print D
    if (time < 1000) {
      Serial.println("d");
    } else {
      Serial.println("D");
    }

    // reset millis to start pause time
    time = millis();
  }
  // if button hasn't already done a long pause, check to see if it has paused for more than a second and print an s
  else if (!printedPause) {
    int time2 = millis() - time;

    // if the button hasn't been pressed for a second or more, print s for long pause
    if (time2 >= 1000){
      Serial.println("s");
      // set printedPause to true so it only prints the s for long pause once after a buton press
      printedPause = true;
    }
  }
}


void problem5e() {
  // if button is pressed, time button press
  if (!(PINC & (1 << BUTTON))) {
    time = millis();
    while(!(PINC & (1 << BUTTON))) {}
    time = millis() - time;
    printedPause = false;

    delay(100);
    if (time < 10) {
      return;
    }

    for(int i = 0; i < 3; i++) {
        last4Morse[i] = last4Morse[i+1];
      }

    // if button pressed less than 1 sec, print d, otherwise print D
    if (time < 1000) {
      Serial.println("d");
      last4Morse[3] = 0;
    } else {
      Serial.println("D");
      last4Morse[3] = 1;
    }

    // if the last 3-4 button presses were r, turn on red led
    if (!memcmp((const void *)last4Morse + 1, (const void *)redMorse, 3)) {
      memset(last4Morse, 3, 4);
      PORTB |= (1 << RED_LED);
      delay(1000);
      PORTB &= ~(1 << RED_LED);
    // if the last 3-4 button presses were g, turn on green led
    } else if (!memcmp((const void *)last4Morse + 1, (const void *)greenMorse, 3)) {
      memset(last4Morse, 3, 4);
      PORTB |= (1 << GREEN_LED);
      delay(1000);
      PORTB &= ~(1 << GREEN_LED);
    // if the last 3-4 button presses were b, turn on blue led
    } else if (!memcmp((const void *)last4Morse, (const void *)blueMorse, 4)) {
      memset(last4Morse, 3, 4);
      PORTB |= (1 << BLUE_LED);
      delay(1000);
      PORTB &= ~(1 << BLUE_LED);
    }

    // reset millis to start pause time
    time = millis();
  }
  // if button hasn't already done a long pause, check to see if it has paused for more than a second and print an s
  else if (!printedPause) {
    int time2 = millis() - time;

    // if the button hasn't been pressed for a second or more, print s for long pause
    if (time2 >= 1000){
      Serial.println("s");
      memset((const void *)last4Morse, 3, 4);
      // set printedPause to true so it only prints the s for long pause once after a buton press
      printedPause = true;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting");
  
  LEDInit();

  problem4Setup();

}

void loop() {
  // problem2();
  // problem4();
  // problem5c();
  // problem5d();
  problem5e();
}
