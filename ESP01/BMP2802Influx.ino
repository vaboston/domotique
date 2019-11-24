// ESP01 + BMP280 6 pins
// Envoie la température a influx toutes les 10s
// Cablage:
//   BMP:
//     - SCL => GPIO2 
//     - SDA => GPIO0
//     - CSB et SDD => Non connecté, pour faire de l'I2C avec l'adresse 0x76
//
// To do:
//  - deep sleep (soudure + code)
//  - envoyer également la pression (duplicate déclaration pour l'instant)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <InfluxDb.h>
#define INFLUXDB_HOST ""
#define INFLUXDB_PORT 0
#define INFLUXDB_USER ""
#define INFLUXDB_PASS ""
#define INFLUXDB_DATABASE ""

Influxdb influx(INFLUXDB_HOST, INFLUXDB_PORT);
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bme; // I2C

const char* ssid = "";
const char* password = "";

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(9600);
  delay(30);
  Wire.begin(0,2);
  if (!bme.begin(0x76)) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);}
  Serial.println(F("BMP280 sensor OK !"));
 
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  influx.setDb(INFLUXDB_DATABASE);
  influx.setDbAuth(INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASS); // with authentication
   
}

void loop() {

   int pressure=(bme.readPressure()/100);
   InfluxData row("alpha_temp");
   row.addTag("device", "alpha");
   row.addTag("sensor", "temp");
   row.addValue("value", bme.readTemperature());
   influx.write(row);
   delay(10000); 
}
