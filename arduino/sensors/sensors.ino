#include <Ultrasonic.h>

#define DEBUG // print meaningful messages to the Serial if defined

// higrometer pin
const int higro_pin = A0;

// sonar pins; [0] is from the front sonar, [1] is for the back sonar
const int trigger_pins[] = {4, 8};
const int echo_pins[] = {5, 9};

Ultrasonic ultra[] = {
  Ultrasonic(trigger_pins[0], echo_pins[0]),
  Ultrasonic(trigger_pins[1], echo_pins[1])
};

void setup() {
  pinMode(higro_pin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  byte byte_read;
  if (Serial.available()) {
    byte_read = Serial.read();
    switch(byte_read)
    {
      case '0': // leitura da umidade do solo
        read_higro();
        break;
      case '1': // leitura da umidade do ar
        read_air();
        break;
      case '2': // leitura da temperatura
        read_temp();
        break;
      case '3': // leitura do sonar dianteiro
        read_sonar(0);
        break;
      case '4': // leitura do sonar traseiro
        read_sonar(1);
        break;
      case '5': // leitura do acelerometro
        read_accel();
        break;
      case '6': // leitura do giroscopio
        read_gyro();
        break;
      case '7': // leitura do GPS
        read_gps();
        break;
      default:
        break;
    }
  }
}

void read_higro() { // 0 - leitura da umidade do solo
  int read_val = analogRead(higro_pin);

# ifdef DEBUG
    Serial.print("higro read_val: ");
    Serial.println(read_val);
# else
    byte low_byte = read_val & 0xFF;
    byte high_byte = (read_val >> 8) & 0xFF;

    Serial.write(low_byte);
    Serial.write(high_byte);
# endif
}

void read_air() { // 1 - leitura da umidade do ar
  Serial.println("(to be implemented) air umidity");
}

void read_temp() { // 2 - leitura da temperatura
  Serial.println("(to be implemented) ambient temp");
}

void read_sonar (int sonar_id) { // 3 e 4 - leitura do sonar dianteiro
  union {
    float f_dist_cm;
    int i_dist_cm;
  } dist_cm;
  long usec = ultra[sonar_id].timing();

  dist_cm.f_dist_cm = ultra[sonar_id].convert(usec, Ultrasonic::CM);
  
# ifdef DEBUG
    char msg[128];
    char sonar_name[][6] = {"front", "back"};
    sprintf(msg, "Sonar %s reading: ", sonar_name[sonar_id]);
    Serial.print(msg);
    Serial.print(dist_cm.f_dist_cm);
    Serial.println(" cm");
# else
    for (int shift = 0; shift <= 24; shift += 8)
      Serial.write((dist_cm.i_dist_cm >> shift) & 0xFF);
# endif
}

void read_accel () { // 5 - leitura do acelerometro
  Serial.println("(to be implemented) accelerometer");
}

void read_gyro() { // 6 - leitura do giroscopio
  Serial.println("(to be implemented) gyroscope");
}

void read_gps() { // 7 - leitura do GPS
  Serial.println("(to be implemented) GPS");
}

