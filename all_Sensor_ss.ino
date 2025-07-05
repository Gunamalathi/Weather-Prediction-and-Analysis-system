#include <SPI.h>
#include <LoRa.h>
#include <ESP8266WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPL3w3sWRGHj"
#define BLYNK_TEMPLATE_NAME "Lora Esp8266"
#define BLYNK_AUTH_TOKEN "5wd0fAd81gkvgH4rmF1-upa5T6PXxlwN"
#include <BlynkSimpleEsp8266.h>

// Blynk Authentication Token
char auth[] = "nPmAvH8sz3mwxzTbP2GUmtGcykbQnKAs";

// Your WiFi credentials
char ssid[] = "User";  // WiFi router name
char pass[] = "User1234";        // Wi-Fi password

String inString = "";    // String to hold input data
String MyMessage = "";

#define Nss 15    // D8 pin CS
#define rst 16    // D0 pin RST
#define dio0 2    // D4 pin DIO0

// Variables to hold sensor values
float Hum_Value = 0.0;
float Temp_Value = 0.0;
float BMP_Temperature = 0.0;
float Pressure = 0.0;
uint16_t Lux = 0;
int RainAnalog = 0;
int RainDigital = 0;

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

  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Read packet
    while (LoRa.available()) {
      int inChar = LoRa.read();
      inString += (char)inChar;
      MyMessage = inString;
    }

    // Parse values from the received message
    String q = getValue(inString, ',', 0);  // Humidity
    String r = getValue(inString, ',', 1);  // Temperature
    String bmpTemp = getValue(inString, ',', 2); // BMP280 Temperature
    String pressureStr = getValue(inString, ',', 3); // BMP280 Pressure
    String luxStr = getValue(inString, ',', 4); // BH1750 Lux
    String rainAnalogStr = getValue(inString, ',', 5); // Rain Sensor Analog
    String rainDigitalStr = getValue(inString, ',', 6); // Rain Sensor Digital

    // Convert strings to float and store values
    Hum_Value = q.toFloat();
    Temp_Value = r.toFloat();
    BMP_Temperature = bmpTemp.toFloat();
    Pressure = pressureStr.toFloat();
    Lux = luxStr.toInt();
    RainAnalog = rainAnalogStr.toInt();
    RainDigital = rainDigitalStr.toInt();

    // Send data to Blynk
    Blynk.virtualWrite(V2, Hum_Value); // Humidity
    Blynk.virtualWrite(V3, Temp_Value); // Temperature
    Blynk.virtualWrite(V4, BMP_Temperature); // BMP280 Temperature
    Blynk.virtualWrite(V5, Pressure); // Pressure
    Blynk.virtualWrite(V6, Lux); // Lux (Light intensity)
    Blynk.virtualWrite(V7, RainAnalog); // Rain Analog
    Blynk.virtualWrite(V8, RainDigital ? "Rain Detected" : "No Rain"); // Rain Digital

    // Reset inString for next packet
    inString = "";
    LoRa.packetRssi();
  }

  // Print received message to Serial Monitor (for debugging)
  Serial.println(MyMessage);
}

// Function to extract data from the received string (separated by commas)
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
return found > index ? data.substring(strIndex[0], strIndex[1]) : ""; 
}
