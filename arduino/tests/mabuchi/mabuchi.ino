const int pwma = 5;
const int ain1 = 6;
const int ain2 = 7;

volatile char read_val;

void setup() {
  pinMode(pwma, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);

  Serial.begin(9600);
  Serial.println("a -> forward\nb -> backward\nc -> stop");
}

void loop() {
  if (Serial.available())
  {
    read_val = Serial.read();
    switch(read_val)
    {
      case 'a':
        move_mabuchi(255, 0);
        break;
      case 'b':
        move_mabuchi(255, 1);
        break;
      case 'c':
        move_mabuchi(0,0);
        break;
      default:
        break;
    }
  }
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

