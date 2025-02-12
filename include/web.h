#ifndef WEB_H
#define WEB_H

#include <ESPAsyncWebServer.h>

// Instanță globală a serverului web
extern AsyncWebServer server;

// Inițializează serverul web și rutele
void initWebServer();

#endif