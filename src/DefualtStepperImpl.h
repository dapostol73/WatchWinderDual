#include <Arduino.h>
#include <Stepper.h>

#define ROT_STEPS 100 // Number of steps
#define ROT_SPEED 300 // Speed of steps

// Create instances of the stepper class
// The motor (wires 1 2 3 4) is connected to the outputs 9 10 11 12 of the Arduino (and to GND, + V)
// Stepper small_stepper(STEPS, 9, 7, 8, 6);  // Anti-clockwise by reversing 8 and 11 (if preferred)
Stepper small_stepper1(ROT_STEPS, 2, 4, 3, 5);      // Clockwise
Stepper small_stepper2(ROT_STEPS, 6, 8, 7, 9);   // Clockwise

// Number of rotation steps requested from the motor.
// One full rotation with 2048 steps (1 turn about 4.5sec)
// To turn upside down 6 times 1 / 30th of a turn, simply multiply steps_to_take by 6/30 and put a minus to reverse the direction
// Example  steps_to_take  = -6*2048/30;
int const steps_to_take = 4096; 

void stepperInitialize() {
    small_stepper1.setSpeed(ROT_SPEED); // Speed ​​of 300 (max) reduce this figure for slower movement
    small_stepper2.setSpeed(ROT_SPEED); // Speed ​​of 300 (max) reduce this figure for slower movement
}

void stepperRun(int motor) {
    switch (motor)
    {
        case 0:
            small_stepper1.step(-steps_to_take);  //It turns
            delay(500);
            small_stepper1.step(steps_to_take);  //It turns
            break;
        case 1:
            small_stepper2.step(steps_to_take);
            delay(500);
            small_stepper2.step(-steps_to_take);
            break;
        default:
            break;
    }
}