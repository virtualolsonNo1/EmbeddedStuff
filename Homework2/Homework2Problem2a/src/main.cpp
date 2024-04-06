/*--------------------------------------------------------------------
Name: 	Carl Olson
Date: 	02/05/2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: 	main.cpp
HW/Lab:	Homework 2

Purp:	Learn about serial communication via creating program to turn on LED when certain buttons are pressed as well as to learn about how delay() can be implemented and how to read a datasheet/schematic


Doc:	No one helped me

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>
//definitions for port B registers
#define PORTB_REG (*((volatile uint8_t *) 0x25))
#define PORTB_DDR (*((volatile uint8_t *) 0x24))
#define PORTB_PINS (*((volatile uint8_t *) 0x23))

#define ONE_KEY_OFFSET 48
#define S_VAL 115
#define F_VAL 102

// value to store serial data
int incomingByte = 0;

// delay time which can be modified by data sent in from user (either s for slow or f for fast)
int delay_time = 600;

// bool so only enter section of a function once
bool firstTime = true;

// counter variables for part 3a
uint8_t counter1 = 0xff;
uint16_t counter2 = 0xffff;
uint32_t counter3 = UINT32_MAX;

// counter variables for part 3b
uint16_t newcounter1 = 0xffff;
uint16_t newcounter2 = 0xffff;
uint8_t newcounter3 = 0xff;
uint8_t newcounter4 = 0xff;
uint8_t newcounter5 = 0xff;
uint32_t newcounter6 = UINT32_MAX;

void problem2() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // print value received from keyboard
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);

    //subtract one lower than the value if 1 is pressed from incomingByte, as shift incomingByte to have values 1-9 when 1-9 are pressed
    incomingByte = incomingByte - ONE_KEY_OFFSET;

    // if we received 1-9, blink that many times
    if (incomingByte >= 1 && incomingByte <= 9) {\
      // loop as many times as incomingByte's value
      for(int i = 0; i < incomingByte; i++) {
      // make PORTB pin 5 go high, turning LED on
      PORTB_REG |= (1 << 5);
      // delay as long as is specified as delay time
      delay(delay_time);

      // make PORTB pin 5 go low by setting 5th bit to 0, turning LED off
      PORTB_REG &= ~(1 << 5);
      delay(delay_time);
    }

    // if 1-9 not sent
    } else {
        // if value is an S (for slow), up delay_time to 1000ms
        if (incomingByte == (S_VAL - ONE_KEY_OFFSET)) {
          delay_time = 1000;
        
        // otherwise, if value is an F (for fast), lower delay_time to 250ms
        } else if (incomingByte == (F_VAL - ONE_KEY_OFFSET)){
          delay_time = 250;
        }
    }
  }
}

void problem3atestcode() {
  if (firstTime) {
    firstTime = false;

    // timing for uint8_t
    int time = micros();
    // get time for 255 iterations
    for (counter1 = 0; counter1 < 255; counter1++) {
      asm volatile("nop");
    }
    int time2 = micros();

    // print timing of for loop above
    Serial.print("Time for 255 loops: ");
    Serial.println(time2 - time, DEC);

    // timing for uint16_t
    time = millis();
    // get time for 65535 iterations
    for (counter2 = 0; counter2 < 0xffff; counter2++) {
      asm volatile("nop");
    }
    time2 = millis();

    // print timing of for loop above
    Serial.print("Time for 65535 loops: ");
    Serial.println(time2 - time, DEC);

    // timing for uint32_t
    time = millis();
    // get time for 65535 iterations
    for (counter3 = 0; counter3 < 0xffff; counter3++) {
      asm volatile("nop");
    }
    time2 = millis();

    // print timing of for loop above
    Serial.print("Time for uint32_t loop: ");
    Serial.println(time2 - time, DEC);
  }
}

void problem3b() {
  if (firstTime) {
    firstTime = false;

    // turn LED on
    PORTB_REG |= (1 << 5);

    // start timing
    int time = millis();

    // delay for 6 seconds
    for (newcounter6 = 0; newcounter6 < 2638998; newcounter6++) {
      asm volatile("nop");
    }

    // stop timing
    int time2 = millis();

    // turn LED off
    PORTB_REG &= ~(1 << 5);
    Serial.print("Time for uint32_t loop: ");
    Serial.println(time2 - time, DEC);

    // delay while off
    delay(200);

    // turn LED on for blink
    PORTB_REG |= (1 << 5);
    delay(200);

    // turn LED off to finish blink
    PORTB_REG &= ~(1 << 5);
    delay(200);

    // turn LED on
    PORTB_REG |= (1 << 5);

    // start timing
    time = millis();

    // delay for 3 seconds
    for(newcounter1 = 0; newcounter1 < 36; newcounter1++) {
      for (newcounter2 = 0; newcounter2 < 0xffff; newcounter2++) {
        asm volatile("nop");
      }
    }
    
    // stop timing
    time2 = millis();
    
    // turn LED off
    PORTB_REG &= ~(1 << 5);
    Serial.print("Time for 65535 loops: ");
    Serial.println(time2 - time, DEC);

    // blink twice
    // delay while off
    delay(200);

    // turn LED on for blink
    PORTB_REG |= (1 << 5);
    delay(200);

    // turn LED off to finish first blink
    PORTB_REG &= ~(1 << 5);
    delay(200);

    // turn LED on for second blink
    PORTB_REG |= (1 << 5);
    delay(200);

    // turn LED off to finish second blink
    PORTB_REG &= ~(1 << 5);
    delay(200);


    // turn LED on
    PORTB_REG |= (1 << 5);

    // start timing
    time = millis();

    // delay for 2 seconds
    for (newcounter3 = 0; newcounter3 < 40; newcounter3++) {
      for (newcounter4 = 0; newcounter4 < 255; newcounter4++) {
        for (newcounter5 = 0; newcounter5 < 255; newcounter5++) {
          asm volatile("nop");
        }
      }
    }

    // stop timing
    time2 = millis();

    // turn LED off
    PORTB_REG &= ~(1 << 5);
    Serial.print("Time for 255 loops: ");
    Serial.println(time2 - time, DEC);
  }
}



void setup() {
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);

  //set PB5 as output pin that defaults low
  PORTB_DDR |= (1 << 5);
  PORTB_REG &= ~(1 << 5);
}

void loop() {
  // problem2();
  // problem3testcode();
  problem3b();
}