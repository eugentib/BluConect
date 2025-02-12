#ifndef SERIAL_H
#define SERIAL_H

#include <Arduino.h>
#include <ESPAsyncTCP.h>

// Inițializează serverul TCP pe portul specificat
void initSerialTCP(uint16_t port);

// Verifică și redirecționează datele primite de la Serial către clienții TCP
void handleSerialData();

// Funcție utilă pentru transmiterea datelor către toți clienții TCP
void sendToTCPClients(uint8_t* data, size_t len);

#endif
