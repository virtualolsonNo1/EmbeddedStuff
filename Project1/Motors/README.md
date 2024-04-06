# Robot Motor Library
## motors.c and motors.h make up the motor library for moving the two 

### Function Declarations and Definitions:
- motorSetup()
    - Arguments: none
    - Return type: void
    - Purpose: sets up motors to be used with timer2 in fast PWM mode with 0xFF as TOP
- void moveForward();
    - Arguments: none
    - Return type: void
    - Purpose: moves robot forward at 144 / 255 for OCR2A and OCR2B and for 870ms given motor setup specified in schematic
- void moveBackward();
    - Arguments: none
    - Return type: void
    - Purpose: moves robot backward at 144 / 255 for OCR2A and OCR2B and for 870ms given motor setup specified in schematic
- void turnRight();
    - Arguments: none
    - Return type: void
    - Purpose: moves robot left motor forward and right motor backward (turning it right in place) at 144 / 255 for OCR2A and OCR2B relative to TOP and for 870ms given motor setup specified in schematic
- void turnLeft();
    - Arguments: none
    - Return type: void
    - Purpose: moves robot left motor backward and right motor forward (turning it left in place) at 144 / 255 for OCR2A and OCR2B relative to TOP and for 870ms given motor setup specified in schematic