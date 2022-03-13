//**************************************************************
// Controller for 4-wire unipolar stepper motors
// tiptopboards.com 05/12/2013
// Driver ULN2003 and motor reduced to 1:64
//
//**************************************************************
//Include stepper.h library
#include <Arduino.h>
#include <AccelStepperImpl.h>

#define DEBUG false // To enable debug logic
#define ROT_COUNT 30 // number of rotations per a cycle
#define PAUSE_COUNT 300 // number of times to pause using in seconds
#define BUTTON1 10 // Pin number to which push button 1 is connected
#define BUTTON2 11 // Pin number to which push button 2 is connected
#define LED1 12 // Pin number to which led 1 is connected
#define LED2 13 // Pin number to which led 2 is connected

// Declaration of variables :
bool serial_init = false;
int counter = 0;
int button_state1 = 0;    // variable that will be used to store the state of button 1
int button_state2 = 0;    // variable that will be used to store the state of button 2

// Rotation time for one turn for debuggin
long rotation_time = 0;
char rotation_info[30];
char rotation_text[100];

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

void serailPrintRotationLine(const long time)
{
    if (DEBUG)
    {
        ltoa(time, rotation_info, 10);
        strcpy(rotation_text, "Rotation Time in MS - ");
        strcat(rotation_text, rotation_info);
        serialPrintLine(rotation_text);
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
    stepperInitialize();
}

void loop()
{
    // reads the state of the button and stores the result in stateButton
    button_state1 = digitalRead(BUTTON1);
    button_state2 = digitalRead(BUTTON2);
    serialPrintLine("Running engine");

    if (counter == 0)
    {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
    }

    if (counter <= ROT_COUNT)
    {
        if (button_state1 == LOW && button_state2 == HIGH)
        {
            serialPrintLine("Spin Watch 1");
            digitalWrite(LED1, HIGH); // Turn on Led 1
            digitalWrite(LED2, LOW); // Turn off Led 2

            rotation_time = millis();
            stepperRun(0);
            rotation_time =  millis() - rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            serailPrintRotationLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(25000);  //pause

            counter++; // Add 1 to the counter
        }
        else if (button_state1 == HIGH && button_state2 == LOW)
        {
            serialPrintLine("Spin Watch 2");
            digitalWrite(LED1, LOW); // Turn off Led 1
            digitalWrite(LED2, HIGH); // Turn on Led 2

            rotation_time = millis();
            stepperRun(1);
            rotation_time =  millis() - rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            serailPrintRotationLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(25000);  //pause

            counter++; // Add 1 to the counter
        }
        else if (button_state1 == LOW && button_state2 == LOW)
        {
            serialPrintLine("Spin Both Watches");
            digitalWrite(LED1, HIGH); // Turn on Led 1
            digitalWrite(LED2, HIGH); // Turn on Led 2

            rotation_time = millis();
            stepperRun(0);
            rotation_time =  millis() - rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            serailPrintRotationLine(rotation_time);      // Displays the rotation_time (in ms) for a full revolution
            delay(2500);  //pause

            rotation_time = millis();
            stepperRun(1);
            rotation_time =  millis() - rotation_time ;  // Timer a full rour 6.236 sec per lap at speed 200
            serailPrintRotationLine(rotation_time); // Displays the rotation_time (in ms) for a full revolution
            delay(2500);  //pause

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
    else if (counter <= (ROT_COUNT + PAUSE_COUNT))
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
            // pause the system for 300 x 2 seconds = 600 seconds 10 minutes
            // should figure out if we want to flash the leds
            if (button_state1 == LOW)
            {
                digitalWrite(LED1, LOW); // Turn off Led 1
            }
            if (button_state2 == LOW)
            {
                digitalWrite(LED2, LOW); // Turn off Led 1

            }
            serialPrintLine("PAUSED");
            delay(1000);
            counter++; // Add 1 to the counter
        }
    }
    else
    {
        counter = 0; // Reset the counter
    }
}