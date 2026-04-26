
#include "network.h"
#include "UserData.h"
#include <string>



Network::Network(UserConfig config)
{
    user_config = config;
    if(user_config.first_run_flag != FIRST_RUN_FLAG_VALUE)
    {
        Serial.println("First run detected, setting up AP mode...\n");
        setup_login_page();
        register_routes();
    }
    else
    {
        Serial.print("Router Id: " + String(user_config.router_id) + "\n");
        Serial.print("Router Secret: " + String(user_config.router_secret) + "\n");
        WiFi.begin(user_config.router_id, user_config.router_secret);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print("....waiting\n");
        }

        Serial.print("Connected to WiFi!\n");
        Serial.print("IP address:" + WiFi.localIP().toString() + "\n");

        IPAddress testIP(10, 0, 0, 199);
        bool pingResult = false;
        while (!pingResult)
        {
            pingResult = Ping.ping(testIP);
            Serial.print("Pinging " + testIP.toString() + ": " + (pingResult ? "Success" : "Failed") + "\n");
            delay(1000);
        }

        if (pingResult)
        {
            int status = send_post_request();
            Serial.print("POST request status: " + String(status) + "\n");
        }
    }

    // Start the server
    server.begin();
}

void Network::register_routes()
{
    server.onNotFound([this]() {
        server.send(404, "text/plain", "Not found");
    });

    // Serve the login page
    server.on("/", [this]() 
    {
        String html = "<html><body><h1>Login</h1><form method='POST' action='/login'><input type='text' name='username' placeholder='Username'><input type='password' name='password' placeholder='Password'><input type='submit' value='Login'></form></body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/register-device", [this]() 
    {
        String html = "<html><body><h1>Register Device</h1><form method='POST' action='/register'><input type='text' name='ssid' placeholder='Router SSID'><input type='password' name='secret' placeholder='Password'><input type='submit' value='Register'></form></body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/login", HTTP_POST, [this]() 
    {
        String username = server.arg("username");
        String password = server.arg("password");

        // Validate credentials (for demonstration, using hardcoded values)
        if(username == DEFAULT_USERNAME && password == DEFAULT_PASSWORD) {
            server.sendHeader("Location", "/register-device", true);
            server.send(303);
        } 
        else
        {
            server.send(401, "text/plain", "Invalid credentials.");
        }
    });

    server.on("/register", HTTP_POST, [this]() 
    {
        String ssid = server.arg("ssid");
        String secret = server.arg("secret");

        Serial.print("Received registration: SSID=" + ssid + ", Secret=" + secret + "\n");

        // Store the credentials (for demonstration, just printing them)
        register_device(ssid.c_str(), secret.c_str());

        server.send(200, "text/plain", "Device registered successfully!");

        delay(2000);
        ESP.restart();
    });
}

void Network::register_device(const char* ssid, const char* secret)
{
    Serial.print("Setting First run flag\n");
    user_config.first_run_flag = FIRST_RUN_FLAG_VALUE;

    Serial.print("Setting Router Credentials\n");
    strncpy(user_config.router_id, ssid, DEFAULT_BUFFER_SIZE);
    strncpy(user_config.router_secret, secret, DEFAULT_BUFFER_SIZE);

    EEPROM.put(FIRST_RUN_FLAG_ADDR, user_config);
    EEPROM.commit();
    EEPROM.end();
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

void Network::setup_login_page()
{
    IPAddress apIP(10, 0, 0, 1);
    IPAddress netMsk(255, 255, 255, 0);

    WiFi.softAPConfig(apIP, apIP, netMsk);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("Pedal-Login");

    dnsServer.start(DNS_PORT, "*", apIP);
}

void Network::http_client()
{
    server.handleClient();
    Serial.println("HTTP server started");
}
