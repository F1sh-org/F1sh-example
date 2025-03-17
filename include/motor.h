#include <stdio.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Modified version of: https://github.com/makerviet/maker-bot

#define MIN_PWM 0
#define MAX_PWM 4095
// PWM channels of pca9685 0-16
#define PWM_CHANNEL1 8
#define PWM_CHANNEL2 9
#define PWM_CHANNEL3 10
#define PWM_CHANNEL4 11
#define PWM_CHANNEL5 12
#define PWM_CHANNEL6 13
#define PWM_CHANNEL7 14
#define PWM_CHANNEL8 15

#define PWM_SERVO1 2
#define PWM_SERVO2 3
#define PWM_SERVO3 4
#define PWM_SERVO4 5
#define PWM_SERVO5 6
#define PWM_SERVO6 7

// Servo PWM Constants
// This value varies depending on the servo motor and PWM Frequency , you should adjust it to fit your servo motor.
#define MIN_SERVO 93
#define MAX_SERVO 600
#define MAX_SERVO_RESERVE 200

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setPWMMotors(int chan, int value)
{
  switch (chan) {
    case 1:
      pwm.setPWM(PWM_CHANNEL1, 0, value);
      break;
    case 2:
      pwm.setPWM(PWM_CHANNEL2, 0, value);
      break;
    case 3:
      pwm.setPWM(PWM_CHANNEL3, 0, value);
      break;
    case 4:
      pwm.setPWM(PWM_CHANNEL4, 0, value);
      break;
    case 5:
      pwm.setPWM(PWM_CHANNEL5, 0, value);
      break;
    case 6:
      pwm.setPWM(PWM_CHANNEL6, 0, value);
      break;
    case 7:
      pwm.setPWM(PWM_CHANNEL7, 0, value);
      break;
    case 8:
      pwm.setPWM(PWM_CHANNEL8, 0, value);
      break;
    default:
      break;
  }
}

void initMotors()
{
  Wire.begin(); // SDA, SCL,400000);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(60);
  Wire.setClock(400000);
  for(int i = 1; i <= 8; i++) {
    setPWMMotors(i, 0);
  }
}
