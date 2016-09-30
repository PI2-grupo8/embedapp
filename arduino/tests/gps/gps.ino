// 5 VDC
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

SoftwareSerial mySerial(3,2);
Adafruit_GPS GPS(&mySerial);

String NMEA1;
String NMEA2;
char c;

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand("$PGCND,33,0*6D"); // Turn off antenna update nuisance data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ); // 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // Request GGA and RMC sentences only
  delay(1000);
}

void loop() {
  readGPS();
}

void readGPS() {

  clearGPS();
  
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA1 = GPS.lastNMEA();

  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA2 = GPS.lastNMEA();

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
    Serial.print(GPS.latitudeDegrees, 4);
    Serial.print(", "); 
    Serial.println(GPS.longitudeDegrees, 4);
    
    Serial.print("Speed (knots): "); Serial.println(GPS.speed);
    Serial.print("Angle: "); Serial.println(GPS.angle);
    Serial.print("Altitude: "); Serial.println(GPS.altitude);
    Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
  }

  
  //Serial.println(NMEA1);
  //Serial.println(NMEA2);
  //Serial.println("");
}

void clearGPS() { // Clear old and corrupt data from Serial Port
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

