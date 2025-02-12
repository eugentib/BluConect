#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>

// Inițializează clientul MQTT cu adresa serverului și portul
void initMQTT(const char* server, uint16_t port);

// Publică un mesaj pe un topic MQTT
void publishMQTT(const char* topic, const char* payload);

#endif
