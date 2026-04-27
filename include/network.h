#pragma once
#ifndef NETWORK_H
#define NETWORK_H


#define NETWORK_PORT 80
#define DNS_PORT 53

#define DEFAULT_USERNAME "admin"
#define DEFAULT_PASSWORD "password"


// Setup for WiFi and Web Server
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266Ping.h>
#include <DNSServer.h>

#include "UserData.h"

// Reaper OSC Setup
#include <WiFiUdp.h>
#include <OSCMessage.h>

class Network 
{
    public:
        bool server_status;
        ESP8266WebServer server;
        DNSServer dnsServer;
        WiFiUDP udp;
        UserConfig user_config;

        Network(UserConfig config);

        void http_client();

        void send_fx_switch();
        void send_wah_position(float wah_percent);
        
    private:

        void setup_login_page();
        void register_routes();
        void register_device(const char* ssid, const char* secret);

        void send_post_request(OSCMessage& message);
        
};
#endif
