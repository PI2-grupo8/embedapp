/*
 * Pins used: A0, A4, A5, 2, 3, 4, 5, 7, 8, 9
 * BAUD rate: 9600
 * 
 * Devices, pins and power:
 *
 *   higrometer (3V3 or 5V):
 *   - Analog pin: A0
 *
 *   DHT22 (3V3 or 5V):
 *   - SIG pin: 7
 *
 *   sonar front (5V):
 *   - TRIG: 4
 *   - ECHO: 5
 *
 *   sonar rear (5V):
 *   - TRIG: 8
 *   - ECHO: 9
 *
 *   IMU (5V):
 *   - SDA: A4
 *   - SCL: A5
 *
 *   GPS (5V):
 *   - TX: 3
 *   - RX: 2
 *
 */

#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
#include <Wire.h>
#include <DHT.h>

#undef DEBUG // print meaningful messages to the Serial if defined

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

/***************** IMU section ***********************************/
// IMU power: 5 VDC
// IMU pins
const int sda_pin = A4;
const int scl_pin = A5;
const int MPU = 0x68;
int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float temp_celsius;

/**************** DHT22 section *********************************/
// DHT22 power: 3V3 or 3V
// DHT type and pins
const int dht_type = DHT22;
const int dht_pin = 7;

DHT dht(dht_pin, dht_type);

void setup() {
  // higrometer setup
  pinMode(higro_pin, INPUT);

  // GPS setup
  GPS.begin(9600);
  GPS.sendCommand("$PGCND,33,0*6D"); // Turn off antenna update nuisance data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz for better stability
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Request GGA and RMC sentences only

  // IMU setup
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);

  Wire.write(0);
  Wire.endTransmission(true);

  // DHT22 setup
  dht.begin();
  
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
  float h = dht.readHumidity();
  while (isnan(h))
    h = dht.readHumidity();
# ifdef DEBUG
  Serial.print("Air umidity: ");
  Serial.print(h, 2);
  Serial.println(" %");
# else
  union {
    float f_hum;
    uint32_t i_hum;
  } hum;
  hum.f_hum = h;
  for (int shift = 0; shift <= 24; shift += 8)
    Serial.write((hum.i_hum >> shift) & 0xFF);
# endif
}

void read_temp() { // 2 - temperature reading
  float t = dht.readTemperature();
  while (isnan(t))
    t = dht.readTemperature();
# ifdef DEBUG
  Serial.print("Temp: "); Serial.println(t);
# else
  union {
    float f_tmp;
    uint32_t i_tmp;
  } tmp;
  tmp.f_tmp = t;
  for (int shift = 0; shift <= 24; shift += 8)
    Serial.write((tmp.i_tmp >> shift) & 0xFF);
# endif
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

void get_imu_data() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  temp_celsius = Tmp/340.00 + 36.53;
}

void read_accel () { // 5 - accelerometer reading
  get_imu_data();
# ifdef DEBUG
  Serial.print("Accel x: "); Serial.println(AcX);
  Serial.print("Accel y: "); Serial.println(AcY);
  Serial.print("Accel z: "); Serial.println(AcZ);
# else
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((AcX >> shift) & 0xFF);
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((AcY >> shift) & 0xFF);
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((AcZ >> shift) & 0xFF);
# endif
}

void read_gyro() { // 6 - gyroscope reading
  get_imu_data();
# ifdef DEBUG
  Serial.print("Gyro X: "); Serial.println(GyX);
  Serial.print("Gyro Y: "); Serial.println(GyY);
  Serial.print("Gyro Z: "); Serial.println(GyZ);
# else
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((GyX >> shift) & 0xFF);
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((GyY >> shift) & 0xFF);
  for(int shift = 0; shift <= 8; shift += 8)
    Serial.write((GyZ >> shift) & 0xFF);
# endif
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

