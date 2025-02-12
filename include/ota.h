#ifndef OTA_H
#define OTA_H

#include <Arduino.h>

// Inițializează endpoint-ul OTA (se adaugă ruta /ota în serverul web)
void initOTA();

// Funcție ce verifică dacă a fost solicitat un update OTA (apelată din loop)
void checkOTAUpdate();

#endif
