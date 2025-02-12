#include "ota.h"
#include "web.h" // Pentru a folosi instanța externă a serverului web
#include <ESP8266HTTPUpdate.h>
#include <LittleFS.h>

static bool otaUpdateRequested = false;
static String otaUrl = ""; // URL-ul de unde se va descărca noul firmware

// Funcția care realizează update-ul OTA
void performOTAUpdate()
{
  WiFiClient client;
  // Dacă nu a fost setat un URL, se folosește unul implicit (acest lucru se poate configura din interfața web)
  if (otaUrl.length() == 0)
  {
    otaUrl = "http://yourserver.com/firmware.bin"; // exemplu de URL
  }
  Serial.println("Începem OTA update...");
  t_httpUpdate_return ret = ESPhttpUpdate.update(client,otaUrl.c_str());

  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.printf("HTTP_UPDATE_FAILED, cod eroare: (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("HTTP_UPDATE_NO_UPDATES");
    break;
  case HTTP_UPDATE_OK:
    Serial.println("HTTP_UPDATE_OK");
    break;
  }
}

// Handler pentru solicitarea OTA prin HTTP POST
void handleOTAUpdate(AsyncWebServerRequest *request)
{
  // Se poate trimite parametrul "url" pentru a specifica noul firmware
  if (request->hasArg("url"))
  {
    otaUrl = request->arg("url");
  }
  otaUpdateRequested = true;
  request->send(200, "text/plain", "OTA update inițiat. Dispozitivul se va reporni dacă update-ul reușește.");
}

void initOTA()
{
  // Adăugăm ruta /ota pe serverul web, ce va declanșa update-ul OTA
  server.on("/ota", HTTP_POST, handleOTAUpdate);
  Serial.println("Endpoint OTA configurat la /ota (metoda POST, parametru 'url').");
}

void checkOTAUpdate()
{
  if (otaUpdateRequested)
  {
    otaUpdateRequested = false; // Resetăm flagul
    performOTAUpdate();
  }
}
