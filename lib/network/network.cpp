
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

    IPAddress testIP(10, 0, 0, 199);
    bool pingResult = false;
    while(!pingResult)
    {
        pingResult = Ping.ping(testIP);
        Serial.print("Pinging " + testIP.toString() + ": " + (pingResult ? "Success" : "Failed") + "\n");
        delay(1000);
    }

    if(pingResult)
    {
        int status = send_post_request();
        Serial.print("POST request status: " + String(status) + "\n");
    }

    // Start the server
    server.begin();
}

int Network::send_post_request()
{
    WiFiUDP udp;
    udp.beginPacket("10.0.0.199", 8000);

    udp.write("Hello, UDP Server!");
    udp.endPacket();

    delay(1000); // Wait for the server to process the packet

    return 0;
}

void Network::http_client()
{
    server.handleClient();
    Serial.println("HTTP server started");
}
