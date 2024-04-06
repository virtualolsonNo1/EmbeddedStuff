/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: main.cpp
HW/Lab: Project 1

Purp: Implements code needed to complete project 1 with L298N, motors, and ultrasonic sensor

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/
#include <Arduino.h>


#define RED_LED PIND5
#define GREEN_LED PIND6
#define BLUE_LED PIND7

#define OCR1B_PWM PINB2
#define ICP1_PIN PINB0
#define TRIGGER_PIN PIND4

#define ENA PIND3
#define IN1 PB1
#define IN2 PB4

#define IN3 PIND2
#define IN4 PINB5
#define ENB PINB3



void LEDInit(){
 //Set pinmode for LEDs to output 
  DDRD |= (1 << RED_LED);
  DDRD |= (1 << GREEN_LED);
  DDRD |= (1 << BLUE_LED);

  //Turn all off
  PORTD &= ~(1 << RED_LED);   //clear output
  PORTD &= ~(1 << GREEN_LED); //clear output
  PORTD &= ~(1 << BLUE_LED);  //clear output

  //Test LEDs
  Serial.println("Testing LEDs...");
  PORTD |= (1 << RED_LED);    //set output
  delay(400);
  PORTD &= ~(1 << RED_LED);   //clear output
  PORTD |= (1 << GREEN_LED);  //set output
  delay(400);
  PORTD &= ~(1 << GREEN_LED); //clear output
  PORTD |= (1 << BLUE_LED);   //set output
  delay(400);
  PORTD &= ~(1 << BLUE_LED);   //clear output
  Serial.println("Finished LED testing!");
  }

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

void motorSetup() {
  // set all as output
  DDRB |= (1 << IN1);
  DDRB |= (1 << IN2);
  DDRD |= (1 << IN3);
  DDRB |= (1 << IN4);

  PORTB &= ~(1 << IN1);    //set output low
  PORTB |= (1 << IN2);    //set output high

  PORTD &= ~(1 << IN3);    //set output low
  PORTB |= (1 << IN4);    //set output high



  // set as output
  DDRD |= (1 << ENA);

  // set as output
  DDRB |= (1 << ENB);

  // Fast PWM with OCR1A as top
  TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);

  // Non-inverting mode for OCR1B and Prescaler 64
  TCCR2B = (1 << CS22);
  // TCCR2C = 0;
  TCNT1 = 0;

  // this give us a 20ms pulse width, as .02 / (1 / 16000000)
  OCR2A = 180;
  // To move servo left, 1ms, so // this give us a 20ms pulse width, as .001 / (1 / 16000000)
  OCR2B = 50;
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

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Setup");
  LEDInit();

  // init timer1 for servo movement with PWM
  servoSetup();

  // init timer1 for ultrasonic sensor timing
  ultrasonicSetup();

  // setup motors with timer 2 and gpio pins for in1-4
  motorSetup();
  
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

  // print ultrasonic reading
  Serial.println(value * 64 / 148);


  // if ICR1 value converted to inches is less than 12, turn on the correct LED(s), otherwise, simply print that value
  if (value * 64 / 148 < 12) {
    Serial.println(value * 64 / 148);

    // regarding direction sent in, 0 represents right, 1 represents the middle, and 2 represents left
    if (direction == 0) {
      // turn on red LED
      PORTD |= (1 << RED_LED);
      delay(1000);
      PORTD &= ~(1 << RED_LED);

    } else if (direction == 2) {
      // turn on blue LED
      PORTD |= (1 << BLUE_LED);
      delay(1000);
      PORTD &= ~(1 << BLUE_LED);
    } else {
      // turn on red and blue LEDs
      PORTD |= (1 << RED_LED);
      PORTD |= (1 << BLUE_LED);
      delay(1000);
      PORTD &= ~(1 << RED_LED);
      PORTD &= ~(1 << BLUE_LED);
    }
  } else {
    Serial.println(value * 64 / 148);
  }
}

void moveForward() {
  // move forwards
  // left motor forward
  PORTB |= (1 << IN1);
  PORTB &= ~(1 << IN2);

  // right motor forward
  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(870);

  PORTB &= ~(1 << IN1);
  PORTB &= ~(1 << IN4);

  delay(500);

}

void moveLeft() {
  // turn left small
  // left motor backwards
  PORTB &= ~(1 << IN1);
  PORTB |= (1 << IN2);
  // right motor forwards
  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN2);
  PORTB &= ~(1 << IN4);
  delay(500);

  // move forwards
  // left motor forward
  PORTB |= (1 << IN1);
  PORTB &= ~(1 << IN2);

  // right motor forward
  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(300);

  PORTB &= ~(1 << IN1);
  PORTB &= ~(1 << IN4);

  delay(500);


  // turn right small
  // left motor forwards
  PORTB |= (1 << IN1);
  PORTB &= ~(1 << IN2);
  // right motor backwards
  PORTD |= (1 << IN3);
  PORTB &= ~(1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN1);
  PORTD &= ~(1 << IN3);
  delay(500);

}

void moveRight() {
  // turn right small
  // left motor forwards
  PORTB |= (1 << IN1); 
  PORTB &= ~(1 << IN2);
  // right motor backwards
  PORTD |= (1 << IN3);
  PORTB &= ~(1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN1);
  PORTD &= ~(1 << IN3);
  delay(500);

  // move forwards
  // left motor forward
  PORTB |= (1 << IN1);
  PORTB &= ~(1 << IN2);

  // right motor forward
  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4); 


  OCR2B = 144;
  OCR2A = 144;
  delay(300);

  PORTB &= ~(1 << IN1);
  PORTB &= ~(1 << IN4);

  delay(500);

  // turn left small
  // left motor backwards
  PORTB &= ~(1 << IN1);
  PORTB |= (1 << IN2); 
  // right motor forwards
  PORTD &= ~(1 << IN3); 
  PORTB |= (1 << IN4); 


  OCR2B = 144;
  OCR2A = 144;
  delay(210);

  PORTB &= ~(1 << IN2);
  PORTB &= ~(1 << IN4);
  delay(500);

}

void newUltrasonicReading() {
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

  // print ultrasonic reading
  Serial.println(value * 64 / 148);

  if (value * 64 / 148 < 12) {
    // turn on red LED
    PORTD |= (1 << RED_LED);
    delay(1000);
    PORTD &= ~(1 << RED_LED);
    moveLeft();

  } else if (value * 64 / 148 > 24) {
    // turn on blue LED
    PORTD |= (1 << BLUE_LED);
    delay(1000);
    PORTD &= ~(1 << BLUE_LED);
    moveRight();
  } else {
    // turn on green LED
    PORTD |= (1 << GREEN_LED);
    delay(1000);
    PORTD &= ~(1 << GREEN_LED);
    moveForward();
  }
}

void robotDance() {
  // move forwards

  // left motor forward
  PORTB |= (1 << IN1); 
  PORTB &= ~(1 << IN2);

  // right motor forward
  PORTD &= ~(1 << IN3);
  PORTB |= (1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(315);

  PORTB &= ~(1 << IN1);
  PORTB &= ~(1 << IN4);

  delay(1000);

  // move backwards
  PORTB &= ~(1 << IN1); 
  PORTB |= (1 << IN2); 

  PORTD |= (1 << IN3); 
  PORTB &= ~(1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(315);
  //157 for 45 turn

  PORTB &= ~(1 << IN2);
  PORTD &= ~(1 << IN3);
  
  delay(1000);



  // turn left small

  // left motor backwards
  PORTB &= ~(1 << IN1); 
  PORTB |= (1 << IN2); 
  // right motor forwards
  PORTD &= ~(1 << IN3);   
  PORTB |= (1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(150);

  PORTB &= ~(1 << IN2);
  PORTB &= ~(1 << IN4);
  delay(1000);

  // turn right small

  // left motor forwards
  PORTB |= (1 << IN1);
  PORTB &= ~(1 << IN2);
  // right motor backwards
  PORTD |= (1 << IN3);
  PORTB &= ~(1 << IN4);


  OCR2B = 144;
  OCR2A = 144;
  delay(150);

  PORTB &= ~(1 << IN1);
  PORTD &= ~(1 << IN3);
  delay(1000);

  // turn left big
  PORTB &= ~(1 << IN1);
  PORTB |= (1 << IN2);

  PORTD &= ~(1 << IN3); 
  PORTB |= (1 << IN4);  


  OCR2B = 144;
  OCR2A = 144;
  delay(315);

  PORTB &= ~(1 << IN2);
  PORTB &= ~(1 << IN4);
  delay(1000);

  // turn right big
  PORTB |= (1 << IN1);   
  PORTB &= ~(1 << IN2);  

  PORTD |= (1 << IN3);    
  PORTB &= ~(1 << IN4);   


  OCR2B = 144;
  OCR2A = 144;
  delay(320);

  PORTB &= ~(1 << IN1);
  PORTD &= ~(1 << IN3);
  delay(5000);
}

void loop() {
  // robotDance();
  newUltrasonicReading();

}
