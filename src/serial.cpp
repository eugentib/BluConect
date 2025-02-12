#include "serial.h"
#include <list>

// Listă statică pentru clienții TCP conectați
static std::list<AsyncClient*> tcpClients;

// Pointerul către serverul TCP
static AsyncServer* tcpServer = nullptr;

// Callback pentru deconectarea unui client
static void onClientDisconnect(void *arg, AsyncClient* client) {
  tcpClients.remove(client);
  Serial.println("Client TCP deconectat!");
}

// Callback pentru recepția datelor de la un client TCP
static void onClientData(void *arg, AsyncClient* client, void *data, size_t len) {
  if (data != nullptr && len > 0) {
    // Transmit datele primite de la client către portul Serial
    Serial.write((uint8_t*)data, len);
  }
}

// Callback apelat la conectarea unui nou client TCP
static void onNewTCPClient(void *arg, AsyncClient* client) {
  Serial.println("Client TCP conectat!");
  tcpClients.push_back(client);
  client->onData(onClientData, NULL);
  client->onDisconnect(onClientDisconnect, NULL);
}

void initSerialTCP(uint16_t port) {
  tcpServer = new AsyncServer(port);
  tcpServer->onClient(onNewTCPClient, NULL);
  tcpServer->begin();
  Serial.printf("Server TCP ascultă pe portul %d\n", port);
}

void handleSerialData() {
  // Dacă există date disponibile de la Serial, citește-le într-un buffer static
  if (Serial.available()) {
    static uint8_t buf[256]; // Buffer static pentru a evita alocările dinamice
    size_t len = Serial.readBytes(buf, sizeof(buf));
    if (len > 0) {
      sendToTCPClients(buf, len);
    }
  }
}

void sendToTCPClients(const uint8_t* data, size_t len) {
  // Trimite datele către toți clienții conectați
  for (auto client : tcpClients) {
    if (client->canSend()) {
      client->write(data, len);
    }
  }
}
