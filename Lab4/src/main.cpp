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
  if (!(PIND & (1 << BUTTON))) {
  Serial.println("LED ON!!!");
  PORTD |= (1 << LED);
  } else {
    PORTD &= ~(1 << LED);
    Serial.println("LED OFF!!!");
  }
}

void buttonPressInterruptNotWorkingCorrect() {
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
  if (PIND & (1 << BUTTON)) {
    cli();
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    asm volatile("nop");
    PORTD &= ~(1 << LED);
    sei();
  }
  
}

void solution2() {
  EICRA = 0x0;
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

