#include "config.h"
#include <LittleFS.h>

// Numele fișierului de configurare
const char *configFileName = "/config.json";

// Definim variabila globală
Settings settings;

bool loadSettings(Settings &settings)
{
    if (!LittleFS.exists(configFileName))
    {
        return false; // Fișierul nu există
    }
    File configFile = LittleFS.open(configFileName, "r");
    if (!configFile)
    {
        return false;
    }
    size_t size = configFile.size();
    if (size > 1024)
    { // Fișierele de configurare mici
        configFile.close();
        return false;
    }
    std::unique_ptr<char[]> buf(new char[size]);
    configFile.readBytes(buf.get(), size);
    configFile.close();

    // Folosim un document JSON static (dimensiunea se poate ajusta)
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, buf.get());
    if (error)
    {
        return false;
    }

    // Citim valorile (dacă lipsesc, se vor folosi valori implicite)
    const char *mqttServer = doc["mqttServer"] | "broker.example.com";
    uint16_t mqttPort = doc["mqttPort"] | 1883;
    strncpy(settings.mqttServer, mqttServer, sizeof(settings.mqttServer));
    settings.mqttServer[sizeof(settings.mqttServer) - 1] = '\0';
    settings.mqttPort = mqttPort;

    return true;
}

bool saveSettings(const Settings &settings)
{
    JsonDocument doc;
    doc["mqttServer"] = settings.mqttServer;
    doc["mqttPort"] = settings.mqttPort;

    File configFile = LittleFS.open(configFileName, "w");
    if (!configFile)
    {
        return false;
    }
    if (serializeJson(doc, configFile) == 0)
    {
        configFile.close();
        return false;
    }
    configFile.close();
    return true;
}
