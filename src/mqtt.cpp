#include "mqtt.h"
#include <AsyncMqttClient.h>
#include <Ticker.h>

static AsyncMqttClient mqttClient;
static Ticker mqttReconnectTimer;

// Exemplu de topic pentru mesaje de diagnostic
static const char* mqttTopicDiagnostics = "esp8266/diagnostics";

// Callback la conectarea la brokerul MQTT
static void onMqttConnect(bool sessionPresent) {
  Serial.println("Conectat la brokerul MQTT.");
  // Dacă e necesar, se pot abona la topicuri (ex. mqttClient.subscribe("esp8266/commands", 0);)
}

// Callback la deconectarea de la brokerul MQTT
static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Deconectat de la brokerul MQTT.");
  // Se reconectează după 30 de secunde, dacă WiFi este activ
  mqttReconnectTimer.once(30, [](){
    Serial.println("Reconectare la MQTT...");
    mqttClient.connect();
  });
}

// Callback la recepția unui mesaj MQTT
static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.printf("Mesaj MQTT primit: topic: %s, payload: %s\n", topic, payload);
  // Aici se pot procesa comenzile primite din MQTT
}

void initMQTT(const char* server, uint16_t port) {
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onMessage(onMqttMessage);
  
  mqttClient.setServer(server, port);
  mqttClient.connect();
}

void publishMQTT(const char* topic, const char* payload) {
  mqttClient.publish(topic, 0, false, payload);
}
