#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#endif

#define NETWORK_PORT 80


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class Network {
    public:
        wl_status_t server_status;
        ESP8266WebServer server;
        

        Network(const char* ssid, const char* password);

        void httpClient();
        
    private:
        
};
