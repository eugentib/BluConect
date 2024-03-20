#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncDNSServer.h>
#include <LittleFS.h>
#include <ArduinoOTA.h>
#include <ESPAsync_WiFiManager.h> // Biblioteca ESPAsync_WiFiManager

#define USE_LITTLEFS true

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len);

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        handleWebSocketMessage(client, data, len);
    }
}

void handleWebSocketMessage(AsyncWebSocketClient *client, uint8_t *data, size_t len)
{
    char msg[len + 1];
    strncpy(msg, (char *)data, len);
    msg[len] = '\0';

    if (strncmp(msg, "SET_BAUD_RATE:", 14) == 0)
    {
        long baudRate = strtol(msg + 14, NULL, 10);
        if (baudRate > 0)
        {
            Serial.begin(baudRate);
            Serial.printf("Viteza portului serial setată la %ld\n", baudRate);
        }
    }
    else
    {
        Serial.println(msg);
    }
}

void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin())
    {
        Serial.println("Eroare la inițializarea LittleFS");
        return;
    }

    // Configurare ESPAsync_WiFiManager
    ESPAsync_WiFiManager wifiManager(&server, NULL, "AutoConnect-FSParams");
    wifiManager.autoConnect("ESPAsync");

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.begin();

    // Configurare OTA
    ArduinoOTA.onStart([]()
                       {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "firmware";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type); });
    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        } });
    ArduinoOTA.begin();
    Serial.println("OTA ready");
}

void loop()
{
    ArduinoOTA.handle();
}
