#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "A23"; // Nombre de tu red WiFi
const char* password = "dneg1050"; // Contraseña de tu red WiFi

const char* serverUrl = "https://servidor-iot-seven.vercel.app/api/data"; // URL del servidor de prueba (cambia la IP según corresponda)

const int humedadSensorPin = 34; // Pin D34 conectado al sensor de humedad
const int lluviaSensorPin = 18; // Pin A0 conectado al sensor de lluvia
const int bombaPin = 2; // Pin D2 conectado a la bomba de agua

const int umbralHumedad = 4000; // Parámetro establecido para la humedad

void setup() {
  
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  pinMode(humedadSensorPin, INPUT);
  pinMode(lluviaSensorPin, INPUT);
  pinMode(bombaPin, OUTPUT);
  digitalWrite(bombaPin, HIGH); // Asegurarse de que la bomba esté apagada inicialmente
}

void loop() {
  int humedad = analogRead(humedadSensorPin);
  int lluvia = analogRead(lluviaSensorPin);
  
  Serial.print("Humedad: ");
  Serial.println(humedad);
  
  Serial.print("Lluvia: ");
  Serial.println(lluvia);
  
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["humedad"] = humedad;
  jsonDoc["lluvia"] = lluvia;

  String jsonString;
  serializeJson(jsonDoc, jsonString);

  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Desconectado");
    
  }

  if (humedad > umbralHumedad) {
    digitalWrite(bombaPin, LOW); // Encender la bomba de agua
    delay(1000); // Mantener la bomba encendida por 2 segundos
    digitalWrite(bombaPin, HIGH); // Apagar la bomba de agua
    Serial.println("Bomba de agua activada");
  }

  delay(10000); // Espera 10 segundos antes de la próxima lectura
}
