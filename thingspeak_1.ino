#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
const char* ssid = "USER";   // your network SSID (name)
const char* password = "user1234";   // your network password
int sensor_pin = A0;
WiFiClient  client;

unsigned long myChannelNumber = 2658628;
const char * myWriteAPIKey = "2EQX09LGHLFRUVW2";



// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;


void setup() {
  pinMode(sensor_pin, INPUT);
  Serial.begin(115200);  //Initialize serial


  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);  // Initialize ThingSpeak
    // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if ((millis() - lastTime) > timerDelay) {

    int sensor_analog = analogRead(sensor_pin);
  int _moisture = ( 100 - ( (sensor_analog / 4095.00) * 100 ) );
  _moisture = _moisture * 3;
  Serial.println( _moisture);
    int x = ThingSpeak.writeField(myChannelNumber, 3, _moisture, myWriteAPIKey);

    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
  delay(1000);
}
