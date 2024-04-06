# Ultrasonic Library
## Ultrasonic.cpp and ultrasonic.h make up and ultrasonic and servo library for moving a servo holding an ultrasonic sensor right, left, or to the middle, and taking readings in each position. 

### Function Declarations and Definitions:
- servoSetup()
    - Arguments: none
    - Return type: void
    - Purpose: sets up servo to be used with timer1
- void ultrasonicSetup();
    - Arguments: none
    - Return type: void
    - Purpose: sets up ultrasonic sensor to be used with timer1, hooked up to PB0 for ICP1 and PD4 for the trigger pin
- void ultrasonicReading(int direction);
    - Arguments: int direction, direction ultrasonic sensor facing at time reading is requested. 
    - Return type: void
    - Purpose: takes reading from ultrasonic sensor in a given direction
- void moveRight();
    - Arguments: none
    - Return type: void
    - Purpose: moves servo to face the right
- void moveMiddle();
    - Arguments: none
    - Return type: void
    - Purpose: moves servo to face the middle
- void moveLeft();
    - Arguments: none
    - Return type: void
    - Purpose: moves servo to face the left
- void servoAndUltrasonicExample();
    - Arguments: none
    - Return type: void
    - Purpose: example code for using servo and ultrasonic with same timer in tandum