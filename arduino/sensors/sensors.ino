/*
 * Pins used: A0, 2, 3, 4, 5, 8, 9
 * 
 */

#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

#define DEBUG // print meaningful messages to the Serial if defined

#ifdef DEBUG
# define ZERO  '0'
# define ONE   '1'
# define TWO   '2'
# define THREE '3'
# define FOUR  '4'
# define FIVE  '5'
# define SIX   '6'
# define SEVEN '7'
#else
# define ZERO   0
# define ONE    1
# define TWO    2
# define THREE  3
# define FOUR   4
# define FIVE   5
# define SIX    6
# define SEVEN  7
#endif

/****************** higrometer section ****************************/
// higrometer power supply: 5 or 3.3 VDC
// higrometer pin
const int higro_pin = A0;

/****************** sonar section *********************************/
// sonar power: 5 VDC
// sonar pins; [0] is from the front sonar, [1] is for the back sonar
const int trigger_pins[] = {4, 8};
const int echo_pins[] = {5, 9};

Ultrasonic ultra[] = {
  Ultrasonic(trigger_pins[0], echo_pins[0]),
  Ultrasonic(trigger_pins[1], echo_pins[1])
};

/***************** GPS section ***********************************/
// GPS power supply: 5 VDC
// GPS pins
const int gps_tx_pin = 3;
const int gps_rx_pin = 2;

SoftwareSerial gpsSerial(gps_tx_pin, gps_rx_pin);
Adafruit_GPS GPS(&gpsSerial);

void setup() {
  // higrometer setup
  pinMode(higro_pin, INPUT);

  // GPS setup
  GPS.begin(9600);
  GPS.sendCommand("$PGCND,33,0*6D"); // Turn off antenna update nuisance data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz for better stability
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Request GGA and RMC sentences only
  
  Serial.begin(9600);
}

void loop() {
  byte byte_read;
  if (Serial.available()) {
    byte_read = Serial.read();
    switch(byte_read)
    {
      case ZERO: // soil moisture reading
        read_higro();
        break;
      case ONE: // air umidity reading
        read_air();
        break;
      case TWO: // temperature reading
        read_temp();
        break;
      case THREE: // front sonar reading
        read_sonar(0);
        break;
      case FOUR: // rear sonar reading
        read_sonar(1);
        break;
      case FIVE: // accelerometer reading
        read_accel();
        break;
      case SIX: // gyroscope reading
        read_gyro();
        break;
      case SEVEN: // GPS reading
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

void read_air() { // 1 - air umidity reading
  Serial.println("(to be implemented) air umidity");
}

void read_temp() { // 2 - temperature reading
  Serial.println("(to be implemented) ambient temp");
}

void read_sonar (int sonar_id) { // 3 e 4 - front and rear sonar reading
  union {
    float f_dist_cm;
    uint32_t i_dist_cm;
  } dist_cm;
  long usec = ultra[sonar_id].timing();

  dist_cm.f_dist_cm = ultra[sonar_id].convert(usec, Ultrasonic::CM);
  
# ifdef DEBUG
    char msg[128];
    char sonar_name[][6] = {"front", "rear"};
    sprintf(msg, "Sonar %s reading: ", sonar_name[sonar_id]);
    Serial.print(msg);
    Serial.print(dist_cm.f_dist_cm);
    Serial.println(" cm");
# else
    for (int shift = 0; shift <= 24; shift += 8) // send the sonar distance one byte at a time LSB first
      Serial.write((dist_cm.i_dist_cm >> shift) & 0xFF);
# endif
}

void read_accel () { // 5 - accelerometer reading
  Serial.println("(to be implemented) accelerometer");
}

void read_gyro() { // 6 - gyroscope reading
  Serial.println("(to be implemented) gyroscope");
}

void read_gps() { // 7 - GPS reading
  char c;
  clearGPS();
  
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence

  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  
# ifdef DEBUG
  Serial.print("\nTime: ");
  Serial.print(GPS.hour, DEC); Serial.print(':');
  Serial.print(GPS.minute, DEC); Serial.print(':');
  Serial.print(GPS.seconds, DEC); Serial.print('.');
  Serial.println(GPS.milliseconds);

  Serial.print("Date: ");
  Serial.print(GPS.day, DEC); Serial.print('/');
  Serial.print(GPS.month, DEC); Serial.print("/20");
  Serial.println(GPS.year, DEC);
  Serial.print("Fix: "); Serial.print((int)GPS.fix);
  Serial.print(" quality: "); Serial.println((int)GPS.fixquality); 
  if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
    Serial.print(", "); 
    Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);
    Serial.print("Location (in degrees, works with Google Maps): ");
    Serial.print(GPS.latitudeDegrees, 6);
    Serial.print(", "); 
    Serial.println(GPS.longitudeDegrees, 6);
    
    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    Serial.print("Angle: "); Serial.println(GPS.angle);
    Serial.print("Altitude: "); Serial.println(GPS.altitude);
    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
  }
# else
  //uint8_t hour, minute, seconds, year, month, day; // the Raspberry already has the time...
  union {
    float f_lat;
    uint32_t u_lat;
  } lat;

  union {
    float f_lon;
    uint32_t u_lon;
  } lon;

  lat.f_lat = GPS.latitudeDegrees;
  lon.f_lon = GPS.longitudeDegrees;

  for (int shift = 0; shift <= 24; shift += 8) // send latitude one byte at a time LSB first
    Serial.write((lat.u_lat >> shift) & 0xFF);

  for (int shift = 0; shift <= 24; shift += 8) // send longitude one byte at a time LSB first
    Serial.write((lon.u_lon >> shift) & 0xFF);
# endif
}

void clearGPS() { // Clear old and corrupt data from Serial Port
  char c;
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence

  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
}

