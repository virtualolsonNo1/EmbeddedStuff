// /*--------------------------------------------------------------------
// Name:   Carl Olson
// Date:   February 15th, 2024
// Course: CSCE 336 Embedded Systems (Spring 2024) 
// File:   lab2_button_count.cpp
// HW/Lab: Lab 2, Button Count Code

// Purp: A brief description of what this program does and 
//     the general solution strategy. 

// Doc:  <list the names of the people who you helped>
//     <list the names of the people who assisted you>

// Academic Integrity Statement: I certify that, while others may have 
// assisted me in brain storming, debugging and validating this program, 
// the program itself is my own work. I understand that submitting code 
// which is the work of other individuals is a violation of the honor   
// code.  I also understand that if I knowingly give my original work to 
// another individual is also a violation of the honor code.
// --------------------------------------------------------------------*/

// #include <Arduino.h>
// //#include <avr/io.h>

// // Definitions for Port B & D registors
// #define PINB_Reg (*((volatile uint8_t *) 0x23))
// #define DDRB_Reg (*((volatile uint8_t *) 0x24))
// #define PORTB_Reg (*((volatile uint8_t *) 0x25))
// #define PIND_Reg (*((volatile uint8_t *) 0x29))
// #define DDRD_Reg (*((volatile uint8_t *) 0x2a))
// #define PORTD_Reg (*((volatile uint8_t *) 0x2b))
// // Definitions for LED assignments:
// #define BOARD_LED 5   //pin 13 is PortB bit 5
// #define RED_LED 1     //pin 9 is PortB bit 1  
// #define GREEN_LED 2  //pin 10 is PortB bit 2
// #define BLUE_LED 3   //pin 11 is PortB bit 3

// #define BUTTON 5 

// // previous TCNT value for determining if button bounced or not
// uint16_t prevVal = TCNT1;


// /**
//  * Init all of the LEDs and test them
//  **/ 
// void LEDInit(){
//  //Set pinmode for LEDs to output 
//   DDRB_Reg |= (1 << BOARD_LED);
//   DDRB_Reg |= (1 << RED_LED);
//   DDRB_Reg |= (1 << GREEN_LED);
//   DDRB_Reg |= (1 << BLUE_LED);

//   //Turn all off
//   PORTB_Reg &= ~(1 << BOARD_LED); //clear output
//   PORTB_Reg &= ~(1 << RED_LED);   //clear output
//   PORTB_Reg &= ~(1 << GREEN_LED); //clear output
//   PORTB_Reg &= ~(1 << BLUE_LED);  //clear output

//   //Test LEDs
//   Serial.println("Testing LEDs...");
//   PORTB_Reg |= (1 << BOARD_LED);  //set output
//   PORTB_Reg |= (1 << RED_LED);    //set output
//   delay(400);
//   PORTB_Reg &= ~(1 << RED_LED);   //clear output
//   PORTB_Reg |= (1 << GREEN_LED);  //set output
//   delay(400);
//   PORTB_Reg &= ~(1 << GREEN_LED); //clear output
//   PORTB_Reg |= (1 << BLUE_LED);   //set output
//   delay(400);
//   PORTB_Reg &= ~(1 << BLUE_LED);   //clear output
//   PORTB_Reg &= ~(1 << BOARD_LED);   //clear output
//   Serial.println("Finished LED testing!");
//   }

// void setup() {                
//   Serial.begin(9600);
//   Serial.println("Starting up...");
//   LEDInit();
//   //Set pinmode for Button as input
//   DDRD_Reg &= ~(1 << BUTTON);
//   //Enable pullup 
//   PORTD_Reg |= (1 << BUTTON);  //set output to enable pullup resistor

  
//   //Init counter1
//   TCCR1A = 0; //Normal mode 0xffff top, rolls over
//   TCCR1B = (1 << CS12) | (1 << CS11); //Clock T1 falling edge
//   TCCR1C = 0;
//   //Set counter to zero, high byte first
//   TCNT1H = 0;
//   TCNT1L = 0;  
//   //Make sure interrupts are disabled 
//   TIMSK1 = 0;
//   TIFR1 = 0;
  
//   Serial.println("Finished setup!");
// }



// void problem21() {
//   // if TCNT1 has increased from the previous iteration, print new value and check if button bounced
//   if (prevVal != TCNT1) {
//     Serial.print("TCNT1: ");
//     Serial.println(TCNT1);

//     // if TCNT1 increased by more than 1, button bounced and print warning accordingly
//     if (TCNT1 - prevVal > 1) {
//       Serial.println("Warning!!! Button bounced!!!!");
//     }
//     prevVal = TCNT1;
//   }
//   delay(100);
// }

// void loop() {
//   problem21();
// }
