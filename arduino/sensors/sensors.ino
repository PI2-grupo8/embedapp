/*
 * Pins used: A0, A4, A5, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 30, 31, 40, 41, 50, 51
 * BAUD rate: 9600
 * 
 * Devices, pins and power:
 *
 *   higrometer (3V3 or 5V):
 *   - Analog pin: A0
 *
 *   AM2302 (3V3 or 5V):
 *   - SIG pin: 7 - 22
 *
 *   sonar front (5V):
 *   - TRIG: 4 - 30
 *   - ECHO: 5 - 31
 *
 *   sonar rear (5V):
 *   - TRIG: 8 - 40
 *   - ECHO: 9 - 41
 *
 *   IMU (5V):
 *   - SDA: A4 - 20
 *   - SCL: A5 - 21
 *
 *   GPS (5V):
 *   - TX: 3 - 50
 *   - RX: 2 - 51
 *   
 *   Mabuchi (Forward):
 *   - PWM: 5
 *   - IN1: 6
 *   - IN2: 7
 *   
 *   Mabuchi (Drilling):
 *   - PWM: 8
 *   - IN1: 9
 *   - IN2: 10
 *   
 *   Stepper (Direction):
 *   - STEP: 11
 *   - DIR:  12
 *   - EN:   13
 */

#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <DHT.h>
#include <Servo.h>

#undef DEBUG // print meaningful messages to the Serial if defined

#ifdef DEBUG

# define HIGRO         '0' // higromter
# define AIR           '1' // air relative humidity
# define TEMP          '2' // temperature
# define FRONT_SONAR   '3' // front sonar
# define REAR_SONAR    '4' // rear sonar
# define ACCEL         '5' // accelerometer
# define GYRO          '6' // gyroscope
# define GPS_M4        '7' // GPS
# define GET_TIME      '8'

# define LEFT_1        'a'
# define LEFT_2        'b'
# define LEFT_3        'c'
# define DN            'd'
# define RIGHT_1       'e'
# define RIGHT_2       'f'
# define RIGHT_3       'g'

# define LEFT_4        'n'
# define RIGHT_4       'o'

# define FORWARD       'h'
# define BACKWARD      'i'
# define STOP          'j'

# define PUSH_DRILL    'k'
# define STOP_DRILL    'l'
# define PULL_DRILL    'm'

#else

# define HIGRO          0
# define AIR            1
# define TEMP           2
# define FRONT_SONAR    3
# define REAR_SONAR     4
# define ACCEL          5
# define GYRO           6
# define GPS_M4         7
# define GET_TIME       8

# define LEFT_1        'a'
# define LEFT_2        'b'
# define LEFT_3        'c'
# define DN            'd'
# define RIGHT_1       'e'
# define RIGHT_2       'f'
# define RIGHT_3       'g'

# define LEFT_4        'n'
# define RIGHT_4       'o'

# define FORWARD       'h'
# define BACKWARD      'i'
# define STOP          'j'

# define PUSH_DRILL    'k'
# define STOP_DRILL    'l'
# define PULL_DRILL    'm'

#endif

/****************** Stepper ***************************************/
const int step_pin = 11;
const int dir_pin = 12;
const int en_pin = 13;

const int stepper_speed = 100;
const int stepper_accel = 100;

const int min_step = 30;
const int mid_step = 60;
const int max_step = 100;
const int big_step = 150;

int cur_pos;
int end_pos;
int rel_pos;
char cur_state;

AccelStepper stepper(1, step_pin, dir_pin);

/****************** Mabuchi ***************************************/
// [0] -> forward
// [1] -> drilling
const int pwm_pin[] = {5, 8};
const int in1_pin[] = {6, 9};
const int in2_pin[] = {7, 10};

/****************** higrometer section ****************************/
// higrometer power supply: 5 or 3.3 VDC
// higrometer pin
const int higro_pin = A0;

/****************** sonar section *********************************/
// sonar power: 5 VDC
// sonar pins; [0] is from the front sonar, [1] is for the back sonar
const int trigger_pins[] = {30, 40};
const int echo_pins[]    = {31, 41};

Ultrasonic ultra[] = {
  Ultrasonic(trigger_pins[0], echo_pins[0]),
  Ultrasonic(trigger_pins[1], echo_pins[1])
};

/***************** GPS section ***********************************/
// GPS power supply: 5 VDC
// GPS pins
const int gps_tx_pin = 50;
const int gps_rx_pin = 51;

SoftwareSerial gpsSerial(gps_tx_pin, gps_rx_pin);
Adafruit_GPS GPS(&gpsSerial);

/***************** IMU section ***********************************/
// IMU power: 5 VDC
// IMU pins
const int sda_pin = 20;
const int scl_pin = 21;
const int MPU = 0x68;
int AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float temp_celsius;

/**************** AM2302 (DHT22) section *********************************/
// DHT22 power: 3V3 or 3V
// DHT type and pins
const int dht_type = DHT22;
const int dht_pin = 22;

DHT dht(dht_pin, dht_type);

void setup() {
  // stepper setup
  pinMode(en_pin, OUTPUT);
  digitalWrite(en_pin, LOW);
  stepper.setMaxSpeed(stepper_speed);
  stepper.setAcceleration(stepper_accel);
  cur_pos = 0;
  rel_pos = 0;
  end_pos = 0;
  cur_state = 0;

  // forward setup
  pinMode(pwm_pin[0], OUTPUT);
  pinMode(in1_pin[0], OUTPUT);
  pinMode(in2_pin[0], OUTPUT);

  // drilling setup
  pinMode(pwm_pin[1], OUTPUT);
  pinMode(in1_pin[1], OUTPUT);
  pinMode(in2_pin[1], OUTPUT);

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
  
  // Initialize serial port
  Serial.begin(9600);
  Serial.flush();
}

void loop() {
  byte byte_read;
  if (Serial.available()) {
    byte_read = Serial.read();
    switch(byte_read)
    {
      case HIGRO: // soil moisture reading
        read_higro();
        break;
      case AIR: // air umidity reading
        read_air();
        break;
      case TEMP: // temperature reading
        read_temp();
        break;
      case FRONT_SONAR: // front sonar reading
        read_sonar(0);
        break;
      case REAR_SONAR: // rear sonar reading
        read_sonar(1);
        break;
      case ACCEL: // accelerometer reading
        read_accel();
        break;
      case GYRO: // gyroscope reading
        read_gyro();
        break;
      case GPS_M4: // GPS reading
        read_gps();
        break;
      case LEFT_4: // L++
        turn_engine(-4);
        break;
      case LEFT_1: // L+
        turn_engine(-3);
        break;
      case LEFT_2: //  L
        turn_engine(-2);
        break;
      case LEFT_3: // L-
        turn_engine(-1);
        break;
      case DN: //DN
        turn_engine(0);
        break;
      case RIGHT_1: // R-
        turn_engine(1);
        break;
      case RIGHT_2: // R
        turn_engine(2);
        break;
      case RIGHT_3 : //R+
        turn_engine(3);
        break;
      case RIGHT_4 : //R++
        turn_engine(4);
        break;
      case FORWARD:
        move_mabuchi(0, 255, 0);
        break;
      case BACKWARD:
        move_mabuchi(0, 255, 1);
        break;
      case STOP:
        move_mabuchi(0, 0, 0);
        break;
      case PUSH_DRILL:
        break;
      case PULL_DRILL:
        break;
      case STOP_DRILL:
        break;
      case GET_TIME:
        send_time();
        break;
      default:
        break;
    }
  }

  if (abs(stepper.distanceToGo()) > 0)
    cur_pos = end_pos - stepper.distanceToGo();
  stepper.run();
}

void read_higro() { // 0 - leitura da umidade do solo
  int read_val = analogRead(higro_pin);

# ifdef DEBUG
    Serial.print("Leitura do higrometro: ");
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
  Serial.print("Umidade relativa do ar: ");
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
  Serial.print("Temperatura: "); Serial.print(t); Serial.println(" C");
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
    char sonar_name[][10] = {"frontal", "traseiro"};
    sprintf(msg, "Sonar %s: ", sonar_name[sonar_id]);
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

void turn_engine(int dir){
  if (cur_state == dir)
    return;
  cur_state = dir;
  switch(dir)
  {
    case -4:
      end_pos = big_step;
      break;
    case -3:
      end_pos = max_step;
      break;
    case -2:
      end_pos = mid_step;
      break;
    case -1:
      end_pos = min_step;
      break;
    case 0:
      end_pos = 0;
      break;
    case 1:
      end_pos = -min_step;
      break;
    case 2:
      end_pos = -mid_step;
      break;
    case 3:
      end_pos = -max_step;
      break;
    case 4:
      end_pos = -big_step;
      break;
    default:
      end_pos = cur_pos;
  }

  rel_pos = end_pos - cur_pos;
  stepper.move(rel_pos);
}

void move_mabuchi(int mot, int mab_speed, int dir)
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
  
  digitalWrite(in1_pin[mot], inPin1);
  digitalWrite(in2_pin[mot], inPin2);
  analogWrite(pwm_pin[mot], mab_speed);
}

void send_time() {
  unsigned long tmp = millis();
  int i;
  
  for (i = 0; i < 4; i++)
    Serial.write((tmp >> (i*8)) & 0xFF);
}

