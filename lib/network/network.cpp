
#include "network.h"
#include <string>



Network::Network(const char* ssid, const char* password) : server(NETWORK_PORT)
{

    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("....waiting\n");
    }

    Serial.print("Connected to WiFi!\n");
    Serial.print("IP address:" + WiFi.localIP().toString() + "\n");

    server.on("/", [this]() {
        server.send(200, "text/plain", "Hello, world!");
    });

    // Start the server
    server.begin();
}

void Network::httpClient()
{
    server.handleClient();
    Serial.println("HTTP server started");
}
