#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "mte";
const char* password = "preta1111";
WebServer server(80);
// Motor driver pins
const int in1 = 25;
const int in2 = 26;
const int ena = 27;
// LEDC (PWM) configuration
const int pwmChannel = 0;
const int pwmFreq = 1000;     // 1 kHz
const int pwmResolution = 8;  // 8-bit resolution (0–255)
void handleControl();
void setup() {
  Serial.begin(115200);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  // Initialize PWM (for latest ESP32 core)
  ledcAttach(ena, pwmFreq, pwmResolution); // Simplified API in v3.x
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  // Define route
  server.on("/control", handleControl);
  server.begin();
  Serial.println("Web server started!");
}
void handleControl() {
  String dir = server.arg("dir");
  int speed = server.arg("speed").toInt();
  speed = constrain(speed, 0, 255);
  if (dir == "F") {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } 
  else if (dir == "R") {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } 
  else if (dir == "S") {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  ledcWrite(ena, speed);  // Works with ledcAttach() in ESP32 core 3.x
  server.send(200, "text/plain", "OK");
}
void loop() {
  server.handleClient();
}
