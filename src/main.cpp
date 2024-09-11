#include "Arduino.h"
#include "Microfire_SHT3x.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <cstdint>
#include <liblwm2m.h>

#include <m2m_client.h>

#include "m2m_server.h"

#include "netstack.hpp"

NetStack net;
Microfire::SHT3x sht30;
extern ESP8266WiFiClass WiFi;
WiFiUDP UDP;

constexpr char srv_address[] = M2M_SERVER_IP;
constexpr uint16_t srv_port  = M2M_PORT;

extern lwm2m_context_t *m2m_init(void);

void get_temp_mC(lwm2m_context_t *m2mH)
{
    // take a measurement
    sht30.measure();

    // display the results
    switch (sht30.status) {
    case sht30.STATUS_NOT_CONNECTED:
    case sht30.STATUS_CRC_ERROR:
        break;

    case sht30.STATUS_NO_ERROR:
        update_temp(m2mH);
        update_humi(m2mH);
        break;
    }
}

void setup()
{
    // start Serial and I2C
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    // start the sensor
    sht30.begin();

    // Start network
    while (NetStack::wifi_init() == -1)
        ;

    net.udp_init();
    net.set_dest(srv_address, srv_port);

    lwm2m_context_t *m2mH = m2m_init();
    uint8_t buffer[1024];

    while (1) {
        time_t step = 10;
        time_t timeout;
        lwm2m_step(m2mH, &step);

        if (m2mH->state == STATE_BOOTSTRAP_REQUIRED) {
            ESP.restart();
        }

        timeout = millis();
        while (millis() - timeout < 5000) {
            int len = net.recv(buffer, sizeof(buffer));
            if (len > 0) {
                Serial.println("Received packet:\n");
                output_buffer(stdout, buffer, len, 1);
                lwm2m_handle_packet(m2mH, buffer, len, &net);
                break;
            }
        }

        get_temp_mC(m2mH);
    }
}

void loop()
{
}
