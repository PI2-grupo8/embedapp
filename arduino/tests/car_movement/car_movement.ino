#include <AccelStepper.h>

const int pinStep = 11;
const int pinDir = 12;
const int pinEnable = 13;

const int motorSpeed = 100;
const int motorAccel = 100;

volatile char readVal = 0;
int val = 0;

AccelStepper motor(1, pinStep, pinDir);

const int pwma = 5;
const int ain1 = 6;
const int ain2 = 7;

volatile char read_val;

void setup() {
  pinMode(pwma, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  
  pinMode(pinEnable, OUTPUT);

  motor.setMaxSpeed(motorSpeed);
  motor.setAcceleration(motorAccel);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available())
  {
    read_val = Serial.read();
    switch(read_val)
    {
      case 'r':
        digitalWrite(pinEnable, LOW);
        val -= 100;
        break;
      case 'l':
        digitalWrite(pinEnable, LOW);
        val += 100;
        break;
      case 'f':
        move_mabuchi(255, 0);
        break;
      case 'b':
        move_mabuchi(255, 1);
        break;
      case 's':
        move_mabuchi(0,0);
        break;
      default:
        break;
    }
  }
  motor.moveTo(val);
  motor.run();
}

void move_mabuchi(int mab_speed, int dir)
{
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;
  
  switch(dir)
  {
    case 0:
      inPin1 = HIGH;
      inPin2 = LOW;
      break;
    case 1:
      inPin1 = LOW;
      inPin2 = HIGH;
      break;
    default:
      return;
  }
  
  digitalWrite(ain1, inPin1);
  digitalWrite(ain2, inPin2);
  analogWrite(pwma, mab_speed);
}

