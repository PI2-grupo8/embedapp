byte byteRead;

void setup() {                
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    byteRead = Serial.read();
    switch(byteRead)
    {
      case 0: // leitura da umidade do solo
      case 1: // leitura da umidade do ar
      case 2: // leitura da temperatura
      case 3: // leitura da 
      case 4:
      default:
    }
  }
}

void read_soil_umidity() {
  
}

void read_air_umidity() {
  
}

void read_temperature() {
  
}

void read_ir1 () {
  
}

void read_ir2 () {
  
}

void read_accel () {
  
}

void read_gyro() {
  
}

void read_gps() {
  
}

