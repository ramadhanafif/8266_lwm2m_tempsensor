#include "m2m_client.h"
#include "liblwm2m.h"
#include <vector>

static lwm2m_context_t *lwm2mH;
constexpr char endpoint_name[] = "HTS";

lwm2m_context_t *m2m_init(void)
{
    lwm2m_object_t *objArray[3];
    objArray[0] = get_security_object(123, "176.9.24.134", NULL, NULL, 0, false);
    Serial.printf("Obj security creation OK\r\n");

    objArray[1] = get_server_object(123, "U", 30, false);
    Serial.printf("Obj server creation OK\r\n");

    objArray[2] = get_object_device();
    Serial.printf("Obj device creation OK\r\n");

    uint16_t objsize = 3;
    Serial.printf("Obj creation OK\r\n");

    lwm2mH = lwm2m_init(NULL);
    if (COAP_NO_ERROR != lwm2m_configure(lwm2mH, endpoint_name, NULL, NULL, objsize, objArray)) {
        Serial.printf("lwm2m_configure failed\r\n");
        assert(0);
    }

    return lwm2mH;
}
