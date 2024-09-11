#include "m2m_client.h"
#include "liblwm2m.h"
#include "m2m_server.h"
#include <vector>

static lwm2m_context_t *lwm2mH;
constexpr char endpoint_name[] = "HTSX";

lwm2m_context_t *m2m_init(void)
{
    lwm2m_object_t *objArray[] = { get_security_object(123, M2M_SERVER_IP, NULL, NULL, 0, false),
                                   get_server_object(123, "U", 30, false),
                                   get_object_device(),
                                   get_object_temp(),
                                   get_object_humi() };

    const uint16_t objsize = sizeof(objArray) / sizeof(lwm2m_object_t *);
    Serial.printf("Obj creation OK\r\n");

    lwm2mH = lwm2m_init(NULL);
    if (COAP_NO_ERROR != lwm2m_configure(lwm2mH, endpoint_name, NULL, NULL, objsize, objArray)) {
        Serial.printf("lwm2m_configure failed\r\n");
        assert(0);
    }

    return lwm2mH;
}
static void print_indent(FILE *stream, int num)
{
    int i;

    for (i = 0; i < num; i++)
        Serial.printf("    ");
}

void output_buffer(FILE *stream, const uint8_t *buffer, size_t length, int indent)
{
    size_t i;

    if (length == 0)
        Serial.printf("\n");

    if (buffer == NULL)
        return;

    i = 0;
    while (i < length) {
        uint8_t array[16];
        int j;

        print_indent(stream, indent);
        memcpy(array, buffer + i, 16);
        for (j = 0; j < 16 && i + j < length; j++) {
            Serial.printf("%02X ", array[j]);
            if (j % 4 == 3)
                Serial.printf(" ");
        }
        if (length > 16) {
            while (j < 16) {
                Serial.printf("   ");
                if (j % 4 == 3)
                    Serial.printf(" ");
                j++;
            }
        }
        Serial.printf(" ");
        for (j = 0; j < 16 && i + j < length; j++) {
            if (isprint(array[j]))
                Serial.printf("%c", array[j]);
            else
                Serial.printf(".");
        }
        Serial.printf("\n");
        i += 16;
    }
    Serial.printf("\n");
}
