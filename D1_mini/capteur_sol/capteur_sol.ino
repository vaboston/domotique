#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#define durationSleep 10 // secondes
int PinSensor = A0;

int ValeurSensor = 0;
int pourcentage = 0;
#define durationSleep 10 // secondes
#include <InfluxDb.h>
#define INFLUXDB_HOST ""
#define INFLUXDB_PORT 80
  //if used with authentication
#define INFLUXDB_USER ""
#define INFLUXDB_PASS ""
#define INFLUXDB_DATABASE ""


Influxdb influx(INFLUXDB_HOST, INFLUXDB_PORT);

const char* ssid = "";
const char* password = "";

// Create an instance of the server
// specify the port to listen on as an argument
   ESP8266WiFiMulti WiFiMulti;
void setup(){
 Serial.begin(9600);
 delay(30);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  influx.setDb(INFLUXDB_DATABASE);


   influx.setDbAuth(INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASS); // with authentication



  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop(){
 ValeurSensor = analogRead(PinSensor);
 pourcentage = ConvertEnPercent(ValeurSensor);
 AfficheValeurEtPourcentage();
   int pressure=(bme.readPressure()/100);
   InfluxData row("serre_hygro");
   row.addTag("device", "alpha");
   row.addTag("sensor", "%");
   row.addValue("value", pourcentage);
   influx.write(row);
 ESP.deepSleep(durationSleep * 1000000);
 }

int ConvertEnPercent(int value){
 int ValeurPorcentage = 0;
 ValeurPorcentage = map(value, 1023, 465, 0, 100);
 return ValeurPorcentage;
}

void AfficheValeurEtPourcentage(){
 Serial.print("Valeur: ");
 Serial.println(ValeurSensor);
 Serial.print("Pourcentage: ");
 Serial.print(pourcentage);
 Serial.println("%");
 Serial.println("-----------");

 }
}
