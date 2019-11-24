// ESP01 + BMP280 6 pins 
// Server web avec Température et Pression
// inspiré de https://crazy-electronic.de/index.php/esp-mit-bmp280
// Cablage :
//   BMP:
//     - SCL => GPIO2 
//     - SDA => GPIO0
//     - CSB et SDD => Non connecté, pour faire de l'I2C avec l'adresse 0x76
//
//
// To do:
//   - deep sleep
//
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bme; 

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(30);

  // BMP init
  Wire.begin(0,2)
  if (!bme.begin(0x76)) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);}
  Serial.println(F("BMP280 sensor OK !"));
 
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

   
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
 
  client.flush();
 
 
  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<h1>Temp and Press web server</h1><br><br> ";
 

  // Send the response to the client

  int pressure=(bme.readPressure());
  client.print(s);
  client.print ("Température :");
  client.print(bme.readTemperature());
  client.print (" C<br>");
  client.print ("Pression &nbsp;&nbsp;    :");
  client.print(pressure/100);
  client.print (" HPa<br>");
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
