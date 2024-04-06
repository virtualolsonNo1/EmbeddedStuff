#include <Arduino.h>

#define BUTTON 2
#define LED 3

void setup() {
  //Set pinmode for Button as input
    DDRD &= ~(1 << BUTTON);
  //set output to enable pullup resistor
    PORTD |= (1 << BUTTON);

    DDRD |= 1 << LED;

    Serial.begin(9600);
    Serial.println("Setup Complete!!!");

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

void loop() {
  // put your main code here, to run repeatedly:
  buttonPress();
}

