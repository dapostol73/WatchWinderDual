//**************************************************************
// Controller for 4-wire unipolar stepper motors
// tiptopboards.com 05/12/2013
// Driver ULN2003 and motor reduced to 1:64
//
//**************************************************************
//Include stepper.h library
#include <Arduino.h>
#include <Stepper.h>

#define DEBUG true // To enable debug logic
#define ROT_STEPS 100 // Number of steps
#define ROT_SPEED 300 // Speed of steps
#define ROT_PAUSE 2000 // time to pause be
#define BUTTON1 10 // Pin number to which push button 1 is connected
#define BUTTON2 11 // Pin number to which push button 2 is connected
#define LED1 12 // Pin number to which led 1 is connected
#define LED2 13 // Pin number to which led 2 is connected

// Declaration of variables :
bool serial_init = false;
int counter = 0;
int button_state1 = 0;    // variable that will be used to store the state of button 1
int button_state2 = 0;    // variable that will be used to store the state of button 2

// Create instances of the stepper class
// The motor (wires 1 2 3 4) is connected to the outputs 9 10 11 12 of the Arduino (and to GND, + V)
// Stepper small_stepper(STEPS, 9, 7, 8, 6);  // Anti-clockwise by reversing 8 and 11 (if preferred)
Stepper small_stepper1(ROT_STEPS, 6, 8, 7, 9);   // Clockwise
Stepper small_stepper2(ROT_STEPS, 2, 4, 3, 5);      // Clockwise

// Number of rotation steps requested from the motor.
// One full rotation with 2048 steps (1 turn about 4.5sec)
// To turn upside down 6 times 1 / 30th of a turn, simply multiply steps_to_take by 6/30 and put a minus to reverse the direction
// Example  steps_to_take  = -6*2048/30;
int const steps_to_take = 4096; 

//long rotation_time = 0; // Rotation time for one turn for debuggin

//************************************************************
// For an engine of this type : http://tiptopboards.com/151-moteur-pas-%C3%A0-pas-r%C3%A9duct%C3%A9-de-5v-4-fils-driver-.html
// 64 steps per revolution, 4 phases, 5.625 ° angle depending on motor specification
// Ratio 1:64 for this mechanically reduced engine
//   360° / 5.625° * 64 = 4096 angles with the gear ratio
//   360° / 5.625° * 64  * 4 coils / 2 bipolar = 2048 step / turn

void serialPrintLine(const String &text)
{
    if (DEBUG && !serial_init)
    {
        Serial.begin(9600);  // 9600 bps
        serial_init = true;
    }
    if (DEBUG)
    {
        Serial.println(text.c_str());
    }
}

void setup()
{
    serialPrintLine("Stepper motor test");
    // initialize that the led pin is an output:
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    // initialize that the button pin is an input :
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    // initialize the stepper motor speed
    small_stepper1.setSpeed(ROT_SPEED); // Speed ​​of 300 (max) reduce this figure for slower movement
    small_stepper2.setSpeed(ROT_SPEED); // Speed ​​of 300 (max) reduce this figure for slower movement
    // 100 allows a high torque > 300 the motor vibrates without turning
}

void loop()
{
    // reads the state of the button and stores the result in stateButton
    button_state1 = digitalRead(BUTTON1);
    button_state2 = digitalRead(BUTTON2);
    serialPrintLine("Running engine");

    if (button_state1 == HIGH)
    {
        digitalWrite(LED1, LOW);
        serialPrintLine("Button 1 State Low");
    }
    if (button_state2 == HIGH)
    {
        digitalWrite(LED2, LOW);
        serialPrintLine("Button 2 State Low");
    }

    if (counter <= 30)
    {
        if (button_state1 == HIGH && button_state2 == LOW)
        {
            serialPrintLine("Spin Watch 1");
            digitalWrite(LED1, HIGH); // Turn on Led 1

            //rotation_time = millis();
            small_stepper1.step(-steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time);      //Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            //rotation_time = millis();
            small_stepper1.step(steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            counter++; // Add 1 to the counter
        }
        else if (button_state1 == LOW && button_state2 == HIGH)
        {
            serialPrintLine("Spin Watch 2");
            digitalWrite(LED2, HIGH); // Turn on Led 2

            small_stepper2.step(steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time);      //Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            //rotation_time = millis();
            small_stepper2.step(-steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            counter++; // Add 1 to the counter
        }
        else if (button_state1 == LOW && button_state2 == LOW)
        {
            serialPrintLine("Spin Both Watches");
            digitalWrite(LED1, HIGH); // Turn on Led 1
            digitalWrite(LED2, HIGH); // Turn on Led 2

            //rotation_time = millis();
            small_stepper1.step(-steps_to_take);  //It turns
            small_stepper2.step(steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            //rotation_time = millis();
            small_stepper1.step(steps_to_take);  //It turns
            small_stepper2.step(-steps_to_take);  //It turns
            //rotation_time =  millis()- rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            //serialPrintLine(rotation_time); // Displays the rotation_time (in ms) for a full revolution
            delay(ROT_PAUSE);  //pause

            counter++; // Add 1 to the counter
        }
        else if (button_state1 == HIGH && button_state2 == HIGH)
        {
            // logic should not happen, if both buttons are off,
            // then we reset the state regardless of counter
            serialPrintLine("System is Disabled");
            counter = 0;
        }
    }
    else if (counter > 30 && counter <= 180)
    {
        if (button_state1 == HIGH && button_state2 == HIGH)
        {
            // logic should not happen, if both buttons are off,
            // then we reset the state regardless of counter
            serialPrintLine("System is Disabled");
            counter = 0;
        }
        else
        {
            // pause the system for 150 x 2 seconds = 300 seconds 6 minutes
            // should figure out if we want to flash the leds
            serialPrintLine("PAUSED");
            delay(ROT_PAUSE);
            counter++; // Add 1 to the counter
        }
    }
    else
    {
        counter = 0; // Reset the counter
    }
}