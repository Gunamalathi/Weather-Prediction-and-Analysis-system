#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
const char* ssid = "User";       // Replace with your WiFi SSID
const char* password = "User1234";   // Replace with your WiFi Password

// Firebase credentials
#define FIREBASE_HOST "https://demo2-2a0cc-default-rtdb.firebaseio.com"  // Replace with your Firebase database URL
#define FIREBASE_AUTH "AIzaSyASwo5qdh9er46xcPgK3n_bfyNXYntmzcw"                   // Replace with your Firebase API key

// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3w3sWRGHj"
#define BLYNK_TEMPLATE_NAME "Lora Esp8266"
#define BLYNK_AUTH_TOKEN "5wd0fAd81gkvgH4rmF1-upa5T6PXxlwN"

// Firebase configuration
FirebaseConfig config;
FirebaseAuth auth;

// Firebase Data object
FirebaseData firebaseData;

// DHT configuration
#define DHTPIN D5          // Pin where the DHT sensor is connected
#define DHTTYPE DHT11      // DHT 11
DHT dht(DHTPIN, DHTTYPE);
char auth[] = BLYNK_AUTH_TOKEN;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  // Initialize DHT sensor
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Serial.println("Firebase initialized.");

  // Initialize Blynk
  Blynk.begin(auth, ssid, password);
  Serial.println("Blynk initialized.");
  }

void loop() {
  Blynk.run();

  // Read data from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if readings are valid
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Send data to Firebase
  if (Firebase.pushFloat(firebaseData, "/Humidity", humidity)) {
    Serial.println("Humidity sent to Firebase");
  } else {
    Serial.print("Failed to send Humidity to Firebase: ");
    Serial.println(firebaseData.errorReason());
  }
  if (Firebase.pushFloat(firebaseData, "/Temperature", temperature)) {
    Serial.println("Temperature sent to Firebase");
  } else {
    Serial.print("Failed to send Temperature to Firebase: ");
    Serial.println(firebaseData.errorReason());
  }
 // Send data to Blynk
  Blynk.virtualWrite(V2, humidity); // Send humidity to virtual pin V2
  Blynk.virtualWrite(V3, temperature); // Send temperature to virtual pin V3

  // Print sensor data to Serial Monitor
  Serial.printf("Humidity: %.2f%%, Temperature: %.2f°C\n", humidity, temperature);

  // Wait before the next reading
  delay(5000); // 5 seconds
}
