#pragma once

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include "IPAddress.h"
#include "wifi_cred.h"

extern ESP8266WiFiClass WiFi;

class NetStack {
  public:
    NetStack()
    {
    }

    static int wifi_init()
    {
        constexpr int delay_ms    = 500;
        constexpr int max_wait_ms = 5000;
        int timer_wait_cnt        = max_wait_ms / delay_ms;

        WiFi.begin(WIFI_SSID, WIFI_PASS);
        Serial.printf("Connecting to %s\n", WIFI_SSID);
        while (WiFi.status() != WL_CONNECTED && timer_wait_cnt != 0) {
            delay(delay_ms);
            timer_wait_cnt--;
        }

        if (timer_wait_cnt == 0) {
            Serial.printf("WiFi connection failed after %d ms\n", max_wait_ms);
            return -1;
        }

        return 0;
    }

    int udp_init()
    {
        return UDP.begin(UDP.localPort());
    }

    int set_dest(IPAddress address, uint16_t port)
    {
        return set_dest(address.toString().c_str(), port);
    }

    int set_dest(const char *address, uint16_t port)
    {
        strncpy(this->address, address, sizeof(this->address));
        this->port = port;
        return 0;
    }

    int send(const uint8_t *data, size_t len)
    {
        UDP.beginPacket(address, port);
        UDP.write(data, len);
        return UDP.endPacket() - 1;
    }

    int recv(uint8_t *buffer, size_t buffer_size)
    {
        int packetSize = UDP.parsePacket();
        if (packetSize) {
            UDP.read(buffer, buffer_size);
        }
        return packetSize;
    }

  private:
    WiFiUDP UDP;
    char address[32];
    uint16_t port;
};
