#include <WiFi.h>   // use <ESP8266WiFi.h> if ESP8266
#include <WiFiClient.h>

const char* ssid = "realme";
const char* password = "Sahil295";

const char* host = "192.168.166.52"; // Server IP (Laptop)
const uint16_t port = 12345;        // Server Port

WiFiClient client;
String name;

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
}

void connectToServer() {
  Serial.print("Connecting to server...");
  while (!client.connect(host, port)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to server.");
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  connectToServer();

  Serial.println("Enter your name: ");
  while (Serial.available() == 0);
  name = Serial.readStringUntil('\n');
  name.trim();

  // Send name to server
  client.println(name);
  Serial.println("Name sent to server: " + name);
}

void loop() {
  // Check for incoming messages
  if (client.available()) {
    String msg = client.readStringUntil('\n');
    Serial.println("Received: " + msg);
  }

  // Check for user input to send
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      client.println(name + ": " + input);
    }
  }

  // Check connection
  if (!client.connected()) {
    Serial.println("Disconnected. Reconnecting...");
    connectToServer();
  }
}
