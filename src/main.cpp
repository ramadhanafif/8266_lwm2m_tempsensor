#include "Arduino.h"
#include "Microfire_SHT3x.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <cstdint>
#include <liblwm2m.h>

#include "netstack.hpp"

NetStack net;
Microfire::SHT3x sht30;
extern ESP8266WiFiClass WiFi;
WiFiUDP UDP;

constexpr char srv_address[] = "176.9.24.134";
constexpr uint16_t srv_port  = 5685;

extern lwm2m_context_t *m2m_init(void);

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
        lwm2m_step(m2mH, &step);

        time_t timeout = millis();
        while (millis() - timeout < 5000) {
            int len = net.recv(buffer, sizeof(buffer));
            if (len > 0) {
                Serial.print("Packet received: ");
                Serial.printf("%s\r\n", buffer);

                lwm2m_handle_packet(m2mH, buffer, len, &net);
            }
        }
    }
}

void loop()

{
    // // take a measurement
    // sht30.measure();
    //
    // // display the results
    // switch (sht30.status)
    //
    // {
    // case sht30.STATUS_NOT_CONNECTED:
    //     Serial.println("Error: Sensor not connected");
    //     break;
    //
    // case sht30.STATUS_CRC_ERROR:
    //     Serial.println("Error: CRC error");
    //     break;
    //
    // case sht30.STATUS_NO_ERROR:
    //     Serial.println((String)sht30.tempC + " °C");
    //     Serial.println((String)sht30.tempF + " °F");
    //     Serial.println((String)sht30.RH + " %RH");
    //     Serial.println((String)sht30.vpd_kPa + " VPD kPa");
    //     Serial.println((String)sht30.dew_pointC + " dew point °C");
    //     Serial.println((String)sht30.dew_pointF + " dew point °F");
    //     Serial.println((String)sht30.heat_indexC + " heat index °C");
    //     Serial.println((String)sht30.heat_indexF + " heat index °F");
    //     Serial.println((String)sht30.wet_bulbC + " wet bulb °C");
    //     Serial.println((String)sht30.wet_bulbF + " wet bulb °F");
    //     break;
    // }

    delay(1000);
}
