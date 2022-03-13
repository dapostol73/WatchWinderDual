#include <AccelStepper.h>

#define ROT_MULTI 5 // Number of times to per a loop 
#define ROT_STEPS 4096 // Number of steps for 1 Rotation
#define ROT_SPEED 1800 // Speed ​​of 300 (max) reduce this figure for slower movement
#define ROT_ACCEL 600 // Speed ​​of 300 (max) reduce this figure for slower movement

// Create instances of the stepper class
// The motor (wires 1 2 3 4) is connected to the outputs 9 10 11 12 of the Arduino (and to GND, + V)
AccelStepper small_stepper1(AccelStepper::HALF4WIRE, 2, 4, 3, 5);      // Clockwise
AccelStepper small_stepper2(AccelStepper::HALF4WIRE, 6, 8, 7, 9);      // Clockwise

// Number of rotation steps requested from the motor.
// One full rotation with 2048 steps (1 turn about 4.5sec)
// To turn upside down 6 times 1 / 30th of a turn, simply multiply steps_to_take by 6/30 and put a minus to reverse the direction
// Example  steps_to_take  = -6*2048/30;
long const steps_to_take = ROT_STEPS * (long)ROT_MULTI;

void stepperInitialize() {
    small_stepper1.setMaxSpeed(ROT_SPEED);
    small_stepper2.setMaxSpeed(ROT_SPEED);
    small_stepper1.setAcceleration(ROT_ACCEL);
    small_stepper2.setAcceleration(ROT_ACCEL);
}

void stepperRun(int motor) {
    switch (motor)
    {
        case 0:
            small_stepper1.runToNewPosition(-steps_to_take);  //It turns
            small_stepper1.runToNewPosition(0);  //It turns
            break;
        case 1:
            small_stepper2.runToNewPosition(steps_to_take);  //It turns
            small_stepper2.runToNewPosition(0);  //It turns
            break;
        default:
            break;
    }
}