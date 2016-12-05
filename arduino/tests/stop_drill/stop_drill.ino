void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  Serial.print("Read val: "); Serial.println(digitalRead(8));
  digitalWrite(13, digitalRead(8));
  delay(100);
}
