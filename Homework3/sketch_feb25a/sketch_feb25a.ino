#include <Arduino.h>

volatile int8_t result_8;
volatile int16_t result_16;
volatile int32_t result_32;
volatile float result_f;
volatile bool firstTime = true;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting");

}

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

void problem4() {
  //Set pinmode for Button as input
  DDRC_Reg &= ~(1 << BUTTON);
  //Enable pullup 
  PORTD_Reg |= (1 << BUTTON);  //set output to enable pullup resistor
}

void loop() {
  problem2();
}
