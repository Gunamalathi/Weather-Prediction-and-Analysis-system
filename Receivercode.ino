#include <SPI.h>
#include <LoRa.h> 
#include <ESP8266WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPL3w3sWRGHj"
#define BLYNK_TEMPLATE_NAME "Lora Esp8266"
#define BLYNK_AUTH_TOKEN "5wd0fAd81gkvgH4rmF1-upa5T6PXxlwN"
#include <BlynkSimpleEsp8266.h>

char auth[] = "5wd0fAd81gkvgH4rmF1-upa5T6PXxlwN";
 
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "User";  // WiFi router name
char pass[] = "User1234"; // password
 
String inString = "";    // string to hold input
String MyMessage = "";
 
#define Nss 15 // D8 pin CS
#define rst 16 // D0 
#define dio0 2 // D4

float Hum_Value = 0.0; 
float Temp_Value = 0.0; 
 
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
   Serial.println("LoRa Receiver");
  LoRa.setPins(Nss, rst, dio0);
  
  if (!LoRa.begin(433E6)) { // or 915E6
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}
 
void loop() {
 
  Blynk.run();
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    // read packet   
    
    while (LoRa.available())
    {
      int inChar = LoRa.read();
      inString += (char)inChar;
      MyMessage = inString;     
    }

String q = getValue(inString, ',', 0); // Humidity
String r = getValue(inString, ',', 1); // Temperature
 
Hum_Value = q.toFloat();
Blynk.virtualWrite(V2,q); // Humidity
Blynk.virtualWrite(V3,r); // Temperature
 
inString = "";     
LoRa.packetRssi();    
  }
      
  Serial.println(MyMessage);  
  
}
 
 
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
