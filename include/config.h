#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ArduinoJson.h>

// Structura setărilor
struct Settings {
  char mqttServer[64];
  uint16_t mqttPort;
  // Se pot adăuga și alte setări (ex.: parametrii WiFi, TCP, etc.)
};

// Variabilă globală cu setările (poate fi inclusă în mai multe fișiere)
extern Settings settings;

// Încarcă setările din fișierul de configurare (returnează true dacă a reușit)
bool loadSettings(Settings &settings);

// Salvează setările în fișierul de configurare (returnează true dacă a reușit)
bool saveSettings(const Settings &settings);

#endif
