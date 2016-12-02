/*
 * Este código gira um determinado ângulo CW com a
 * e CCW com b. Para com c.
 */

#include <AccelStepper.h>

const int pinStep = 11;
const int pinDir = 12;
const int pinEnable = 13;

const int motorSpeed = 100;
const int motorAccel = 100;

volatile char readVal = 0;
int val = 0;

AccelStepper motor(1, pinStep, pinDir);

void setup() {
  Serial.begin(9600);
  pinMode(pinEnable, OUTPUT);

  motor.setMaxSpeed(motorSpeed);
  motor.setAcceleration(motorAccel);
  
  Serial.println("a -> Cw\nb -> CCW\nc -> STOP");
}

void loop() {
  if (Serial.available() > 0)
  {
    readVal = Serial.read();
    switch(readVal)
    {
      case 'a':
        digitalWrite(pinEnable, LOW);
        val += 100;
        break;
      case 'b':
        digitalWrite(pinEnable, LOW);
        val -= 100;
        break;
      case 'c':
        motor.moveTo(0);
        digitalWrite(pinEnable, HIGH);
        break;
      default:
        break;
    }
  }
  motor.move(val);
  motor.run();
}
