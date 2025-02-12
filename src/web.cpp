#include "web.h"
#include "config.h"  // Pentru acces la variabila globală "settings"
#include <LittleFS.h>

AsyncWebServer server(80);

// Handler pentru POST-ul datelor de configurare (salvează noile setări)
void handleConfigPost(AsyncWebServerRequest *request) {
  if (request->hasArg("mqttServer") && request->hasArg("mqttPort")) {
    String mqttServer = request->arg("mqttServer");
    String mqttPortStr = request->arg("mqttPort");
    
    // Actualizează setările din memoria globală
    mqttServer.toCharArray(settings.mqttServer, sizeof(settings.mqttServer));
    settings.mqttPort = mqttPortStr.toInt();
    
    // Salvează setările în fișierul de configurare
    if (saveSettings(settings)) {
      request->send(200, "text/plain", "Setări salvate. Dispozitivul se va reporni acum.");
      delay(1000);
      ESP.restart();
    } else {
      request->send(500, "text/plain", "Eroare la salvarea setărilor.");
    }
  } else {
    request->send(400, "text/plain", "Parametri MQTT lipsesc.");
  }
}

void initWebServer() {
  // Servește toate fișierele statice din LittleFS, cu index.html ca fișier implicit
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  // Endpoint POST pentru actualizarea setărilor de configurare
  server.on("/config", HTTP_POST, handleConfigPost);

  server.begin();
  Serial.println("Serverul web a pornit.");
}
