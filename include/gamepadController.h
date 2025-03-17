#include <Arduino.h>
#include <F1sh.h>

// Credit: https://github.com/makerviet/maker-bot

#define DEBUG

#define TOP_SPEED 4095
#define NORM_SPEED 2048
#define PRECISE_SPEED 2048
#define TURNING_FACTOR 1

#define SINGLE_HAND_DRIVING 0
#define TWO_HAND_DRIVING 1

bool driving_mode = SINGLE_HAND_DRIVING;
int speed = NORM_SPEED;
int c1 = 0, c2 = 0, c3 = 0, c4 = 0;

void gamepadControl(float X, float Y, float buttons[][17]){
    if (buttons[0][0] == 1.00){
        speed = TOP_SPEED;
    } else if (buttons[0][1] == 1.00){
        speed = PRECISE_SPEED;
    } else {
        speed = NORM_SPEED;
    }
    int nJoyX = 128 - X;
    int nJoyY = 128 - Y;
    int nMotMixL;                          // Motor (left) mixed output
    int nMotMixR;                          // Motor (right) mixed output

    bool temp = (nJoyY * nJoyX > 0);
    if (nJoyX) // Turning
    {
        nMotMixL = -nJoyX + (nJoyY * temp);
        nMotMixR = nJoyX + (nJoyY * !temp);
    }
    else // Forward or Reverse
    {
        nMotMixL = nJoyY;
        nMotMixR = nJoyY;
    }
    #ifdef DEBUG
    Serial.print(F("Calculated value from joystick: "));
    Serial.print(nMotMixL);
    Serial.print("\t");
    Serial.println(nMotMixR);
    #endif
    c1 = 0, c2 = 0, c3 = 0, c4 = 0;
    if (nMotMixR > 0)
    {
        c3 = nMotMixR;
        c3 = map(c3, 0, 128, 0, speed);
    }

    else if (nMotMixR < 0)
    {
        c4 = abs(nMotMixR);
        c4 = map(c4, 0, 128, 0, speed);
    }

    if (nMotMixL > 0)
    {
        c1 = nMotMixL;
        c1 = map(c1, 0, 128, 0, speed);
    }
    else if (nMotMixL < 0)
    {
        c2 = abs(nMotMixL);
        c2 = map(c2, 0, 128, 0, speed);
    }
    setPWMMotors(1, c1);
    setPWMMotors(2, c2);
    setPWMMotors(3, c3);
    setPWMMotors(4, c4);
}