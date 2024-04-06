/*--------------------------------------------------------------------
Name:   Carl Olson
Date:   February 18th, 2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File:   lab2_button_count.cpp
HW/Lab: Lab 2, Button Count Code

Purp: Calculates time of a button debounce as well as how long a button was pressed in order to introduce students to the timer peripheral and its different use cases

Doc:  N/A

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/

#include <Arduino.h>
//#include <avr/io.h>

// Definitions for Port B & D registors
#define PINB_Reg (*((volatile uint8_t *) 0x23))
#define DDRB_Reg (*((volatile uint8_t *) 0x24))
#define PORTB_Reg (*((volatile uint8_t *) 0x25))
#define PIND_Reg (*((volatile uint8_t *) 0x29))
#define DDRD_Reg (*((volatile uint8_t *) 0x2a))
#define PORTD_Reg (*((volatile uint8_t *) 0x2b))
// Definitions for LED assignments:
#define BOARD_LED 5   //pin 13 is PortB bit 5
#define RED_LED 1     //pin 9 is PortB bit 1  
#define GREEN_LED 2  //pin 10 is PortB bit 2
#define BLUE_LED 3   //pin 11 is PortB bit 3

#define BUTTON 5
#define BUTTON2 0 
#define PRESCALER64_SEC 0.000004
#define PRESCALER1024_SEC 0.000064

// previous TCNT value for determining if button bounced or not
uint16_t prevVal = TCNT1;

// global variable for how many times button has been pressed
int buttonCount = 0;

// global test count variable to figure out timing of each prescaler
uint16_t count = 0;


/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
 //Set pinmode for LEDs to output 
  DDRB_Reg |= (1 << BOARD_LED);
  DDRB_Reg |= (1 << RED_LED);
  DDRB_Reg |= (1 << GREEN_LED);
  DDRB_Reg |= (1 << BLUE_LED);

  //Turn all off
  PORTB_Reg &= ~(1 << BOARD_LED); //clear output
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg &= ~(1 << BLUE_LED);  //clear output

  //Test LEDs
  Serial.println("Testing LEDs...");
  PORTB_Reg |= (1 << BOARD_LED);  //set output
  PORTB_Reg |= (1 << RED_LED);    //set output
  delay(400);
  PORTB_Reg &= ~(1 << RED_LED);   //clear output
  PORTB_Reg |= (1 << GREEN_LED);  //set output
  delay(400);
  PORTB_Reg &= ~(1 << GREEN_LED); //clear output
  PORTB_Reg |= (1 << BLUE_LED);   //set output
  delay(400);
  PORTB_Reg &= ~(1 << BLUE_LED);   //clear output
  PORTB_Reg &= ~(1 << BOARD_LED);   //clear output
  Serial.println("Finished LED testing!");
  }

void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up...");
  LEDInit();

  //Set pinmode for Button as input
  DDRD_Reg &= ~(1 << BUTTON);
  //Enable pullup 
  PORTD_Reg |= (1 << BUTTON);  //set output to enable pullup resistor
  //Set pinmode for Button as input
  DDRB_Reg &= ~(1 << BUTTON2);
  //Enable pullup 
  PORTB_Reg |= (1 << BUTTON2);  //set output to enable pullup resistor

  
  //Init counter1
  TCCR1A = 0; //Normal mode 0xffff top, rolls over
  // set prescaler (in this case 1024)
  TCCR1B &= ~(1 << CS11);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // set rising or falling edge, in this case rising
  TCCR1B |= (1 << ICES1);
  TCCR1C = 0;
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  //Make sure interrupts are disabled 
  TIMSK1 = 0;
  TIFR1 = 0;
  
  Serial.println("Finished setup!");
}



void problem21() {
  // if TCNT1 has increased from the previous iteration, print new value and check if button bounced
  if (prevVal != TCNT1) {
    // if TCNT1 increased by more than 1, button bounced and print warning accordingly
    if (TCNT1 - prevVal > 1) {
      Serial.println("Warning!!! Button bounced!!!!");
    }

    // print TCNT1 value if changed from previous iteration
    Serial.print("TCNT1: ");
    Serial.println(TCNT1);
    prevVal = TCNT1;
  }
  delay(100);
}

// test function associated with checkoff 2 to check if my calculations were right
void problem22() {
  if (TCNT1 == UINT16_MAX) {
    count++;
    if (count % 120 == 0) {
      Serial.println("HIT 30 SECONDS!!!!!!");
    }
  }
}

void problem23() {
  // if TIFR1 flag set at bit 5, button was pressed
  if (TIFR1 & (1 << 5)) {
    // increment buttonCount
    buttonCount++;
    //clear interrupt flag so doesn't re-enter and clear ICR1 and TCNT1 to start counting up from 0
    TIFR1 |= (1 << 5);
    ICR1 = 0;
    TCNT1 = 0;
    // delay for 200 ms as to allow for bounce
    delay(200);
    float value = ICR1;
    // if ICR1 is not 0, button bounced upon release, otherwise 
    if (value != 0) {
      Serial.println("BUTTON BOUNCED");
      Serial.print("Time elapsed: ");
      Serial.println(PRESCALER64_SEC * value);
    } else {
      Serial.println("NO BUTTON BOUNCE!!!!");
      Serial.print("Button Press Count: ");
      Serial.println(buttonCount);
    }
  }
}

void problem24() {
  // wait until button pressed and then released
  while (PINB_Reg & (1 << BUTTON2)) {}
  while (!(PINB_Reg & (1 << BUTTON2))) {}
  // increment buttonCount
  buttonCount++;
  // clear ICR1 and TCNT1
  ICR1 = 0;
  TCNT1 = 0;
  // delay for 200 ms to allow button to bounce
  delay(200);
  float value = ICR1;

  // if ICR1 is not 0, the button bounced, otherwise, simply print button count
  if (value != 0) {
    Serial.println("BUTTON BOUNCED");
    Serial.print("Time elapsed: ");
    Serial.println(PRESCALER64_SEC * value);
  } else {
    Serial.println("NO BUTTON BOUNCE!!!!");
    Serial.print("Button Press Count: ");
    Serial.println(buttonCount);
  }
}

void problem25() {
  // wait until button pressed and then released
  while (PINB_Reg & (1 << BUTTON2)) {}
  ICR1 = 0;
  TCNT1 = 0;
  while (!(PINB_Reg & (1 << BUTTON2))) {}
  // increment buttonCount
  buttonCount++;

  float value = ICR1;
  Serial.println("Button push time elapsed");
  Serial.println(PRESCALER1024_SEC * value);
}

void loop() {
  // problem21();
  // problem22();
  // problem23();
  // problem24();
  problem25();
}
