/*--------------------------------------------------------------------
Name: 	Carl Olson
Date: 	02/05/2024
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: 	main.cpp
HW/Lab:	Lab 1

Purp:	Learn about rgb led's, photoresistors, servo motors, and introduce PWM by moving a motor 180 degrees and back


Doc:	No one helped me

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
--------------------------------------------------------------------*/
#include <Arduino.h>
#include <Servo.h>

#define RED PD5
#define GREEN PD6
#define BLUE PD7
#define BUTTONPIN A5
#define PHOTORES1 A0
#define PHOTORES2 A1

Servo myservo; 

int pos = 0;
int buttonState = 0;

//variables to store output value from photoresistors and direction last moved
int val = 0;
int val2 = 0;
bool prevDirection = false;

//variable to store current Position of motor relative to start
int currPos = 1;

// checkoff 2 code
void changeRGBLight() {
  // read in value of button
  buttonState = digitalRead(BUTTONPIN);

  // if the button is pressed and goes low, light up LED in following pattern
  if(buttonState == LOW) {
    //R
    digitalWrite(RED, HIGH);
    delay(1000);
    //RG
    digitalWrite(GREEN, HIGH);
    delay(1000);
    //G
    digitalWrite(RED, LOW);
    delay(1000);
    //GB
    digitalWrite(BLUE, HIGH);
    delay(1000);
    //B
    digitalWrite(GREEN, LOW);
    delay(1000);
    //RB
    digitalWrite(RED, HIGH);
    delay(1000);
    //Turn off colors that were on
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, LOW);
  }
}

// checkoff 3 and 4 code
void readPhotoSensor() {
  // read in value from photoresistor
  val = analogRead(PHOTORES1);

  // print analog value
  Serial.println(val);
}

// checkoff 5 code
void moveServo() {
  // read in state of the button
  buttonState = digitalRead(BUTTONPIN); 

  // if button pressed and buttonState goes low, move the servo motor 180 degrees and back
  if (buttonState == LOW) {
    Serial.println("Button Pressed");
    for (pos = 0; pos <= 180; pos += 1) {
      myservo.write(pos); 
       // waits 15ms for the servo to reach the position
      delay(15);
    }

    //move the servo motor back 180 degrees to get to starting position
    for (pos = 180; pos >= 0; pos -= 1) {
      myservo.write(pos);
      
       // waits 15ms for the servo to reach the position
      delay(15);              
    }
  }
}

// checkoff 6 code
void twoPhotoresistorBrightest() {
  // read in values from photoresistor
  val = analogRead(PHOTORES1);
  val2 = analogRead(PHOTORES2);

  //if one photoresistor is greater than other, brighter light so move there
  if (val >= val2 && currPos > 0) {
    currPos--;
    myservo.write(currPos);
    delay(15); 

  //otherwise, move towards other one
  } else if (val2 > val && currPos < 180) {
    currPos++; 
    myservo.write(currPos);
    delay(15);

  }


}

// checkoff 7 code
void onePhotoResistorBrightest() {
  // move to check to one side to see photoresistor value there
  for(int i = 0; i < 5; i++) {
    if (currPos == 0) {
      continue;
    }

    // decrement current position and move servo motor there
    currPos--;
    myservo.write(currPos);
    delay(15); 

  }

  // store photoresistor value in current position
  val = analogRead(PHOTORES2);
  Serial.println(currPos);

  // move other direction, hence double value in for loop relative to the above loop
  for(int i = 0; i < 10; i++) {
    //confining the motor to move between 0-180 for this section when following light for ease of testing/programming
    if (currPos == 180) {
      continue;
    }

    // increment current position and move servo motor there
    currPos++;
    myservo.write(currPos);
    delay(15); 

  }

  // store value from photoresistor in curretn position
  val2 = analogRead(PHOTORES2);

  // if where we're currently at has bigger value and is therefore brighter, we're good to do this whole process again from here
  if (val2 > val) {
    prevDirection = false;

    // otherwise, move back to other direction as it's brighter over there
  } else if(val > val2) {
    prevDirection = true;
    for(int i = 0; i < 10; i++) {
      if (currPos == 0) { 
      continue;
    }

      currPos--;
      myservo.write(currPos);
      delay(15); 

    } 
    // or, if they are equal, keep moving in whatever direction it went last time
  } else if (val2 == val) {
    if(prevDirection) {
      for(int i = 0; i < 10; i++) {
        // confine range of movement to 0-180
      if (currPos == 0) { 
      continue;
    }

      currPos--;
      myservo.write(currPos);
      delay(15); 

    } 

    }
  }

}

void setup() {

  // initialize LED pins as outputs
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // attaches the servo on pin PD2 to the servo object
  myservo.attach(PD2);
  
  // initialize button pin as an input
  pinMode(BUTTONPIN, INPUT);

  //initialize pin connected to photoresistor as input
  pinMode(PHOTORES1, INPUT);
  pinMode(PHOTORES2, INPUT);

  //start serial so can view serial output in terminal
  Serial.begin(9600);
}

void loop() {
  // different function calls associated with the parts of this lab
  // moveServo();
  // readPhotoSensor();
  // changeRGBLight();
  // twoPhotoresistorBrightest();
  // onePhotoResistorBrightest();
}