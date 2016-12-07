#define BRAKEVCC 0
#define CW 1
#define CCW 2
#define BRAKEGND 3
#define CS_THRESHOLD 15

 
const int inApin = 9;
const int inBpin = 10;
int pwmpin = 8;
int cspin = A1;

int statpin = 13;

int i = 0;

void setup()
{
  pinMode(statpin, OUTPUT);
  
  pinMode(inApin, OUTPUT);
  pinMode(inBpin, OUTPUT);
  pinMode(pwmpin, OUTPUT);
  
  digitalWrite(inApin, LOW);
  digitalWrite(inBpin, LOW);

  Serial.begin(9600);
}

void loop()
{
  char byteRead;
  if(Serial.available() > 0) {
    byteRead = Serial.read();

    switch(byteRead) {
      case 'a':
        motorGo(CW, 255);
        break;
      case 'b':
        motorGo(CW, 0);
        break;
      case 'c':
        motorGo(CCW, 255);
        break;
      default:
        break;
    }
    if (analogRead(cspin) > CS_THRESHOLD)
      motorOff();
  }
  digitalWrite(statpin, LOW);
}

void motorOff()     //Função para desligar o motor se o mesmo travar
{
  digitalWrite(inApin, LOW);
  digitalWrite(inBpin, LOW);

  analogWrite(pwmpin, 0);
  
  digitalWrite(statpin, HIGH);
}

void motorGo(uint8_t direct, uint8_t pwm)
{
  if (direct) {
    digitalWrite(inApin, HIGH);
    digitalWrite(inBpin, LOW);
  } else {
    digitalWrite(inApin, LOW);
    digitalWrite(inBpin, HIGH);
  }

  analogWrite(pwmpin, pwm);
}
