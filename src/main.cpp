#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <LittleFS.h>

#include "config.h"
#include "web.h"
#include "mqtt.h"
#include "serial.h"
#include "ota.h"

// Setările implicite pentru WiFi (poți extinde și aici după necesități)
const char* ssid     = "numele_retelei";
const char* password = "parola_retelei";

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n--- Start ESP8266 Project ---");

  // Inițializare LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Eroare la montarea LittleFS!");
    return;
  }
  
  // Încărcăm setările din fișierul de configurare
  if (!loadSettings(settings)) {
    Serial.println("Nu s-au găsit setări, se folosesc valorile implicite.");
    // Setări implicite pentru MQTT:
    strncpy(settings.mqttServer, "broker.example.com", sizeof(settings.mqttServer));
    settings.mqttPort = 1883;
    // Salvăm valorile implicite pentru viitoarele restartări
    saveSettings(settings);
  }
  Serial.print("MQTT server: ");
  Serial.println(settings.mqttServer);
  Serial.print("MQTT port: ");
  Serial.println(settings.mqttPort);

  // Conectare la rețeaua WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectare la WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectat, IP: ");
  Serial.println(WiFi.localIP());

  // Inițializare serverul web (include ruta pentru setări)
  initWebServer();
  
  // Inițializare client MQTT cu parametrii din setări
  initMQTT(settings.mqttServer, settings.mqttPort);
  
  // Inițializare adaptor Serial-TCP
  initSerialTCP(23); // Exemplu de port TCP, poate fi configurabil și el
  
  // Inițializare OTA
  initOTA();
}

void loop() {
  // Gestionăm datele de la Serial (le trimitem către clienții TCP)
  handleSerialData();
  
  // Verificăm dacă s-a solicitat un update OTA
  checkOTAUpdate();
  
  // Alte task-uri pot fi adăugate aici...
}
