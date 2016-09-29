// Vcc = 5V

// Must be installed in Arduino/libraries
// (download link: https://github.com/filipeflop/Ultrasonic/archive/master.zip)
#include <Ultrasonic.h>

const int trigger_pin = 4;
const int echo_pin = 5;

Ultrasonic ultra(trigger_pin, echo_pin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float dist_cm;
  long usec = ultra.timing();

  dist_cm = ultra.convert(usec, Ultrasonic::CM);

  Serial.print("distance in cm: ");
  Serial.println(dist_cm);
}
