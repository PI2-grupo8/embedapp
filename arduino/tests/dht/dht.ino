// Power: 3V3 or 5V
#include <DHT.h>

const int dht_type = DHT22;
const int dht_pin = 8;

DHT dht(dht_pin, dht_type);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 test!");

  dht.begin();
}

void loop() {
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.println(" *C ");

}
