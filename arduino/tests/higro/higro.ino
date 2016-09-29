// Vcc = 3.3V or 5V

const int higro_pin = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int higro_read = analogRead(higro_pin);

  byte low = higro_read & 0xFF;
  byte high = (higro_read >> 8) & 0xFF;

  Serial.write(low);
  Serial.write(high);
}

