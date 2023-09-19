#include "Microfire_SHT3x.h"
#include "liblwm2m.h"
#include "m2m_client.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Resource Id's:
#define RES_M_SENSOR_VALUE 5700
#define RES_O_UNITS        5701
#define OBJECT_humi_UNIT   "Percent"

extern Microfire::SHT3x sht30;
void update_humi(lwm2m_context_t *contextP)
{
    static lwm2m_uri_t uri_humi = { .objectId = 3304, .instanceId = 0, .resourceId = LWM2M_MAX_ID };
    lwm2m_resource_value_changed(contextP, &uri_humi);
}

static uint8_t prv_humi_read(
    lwm2m_context_t *contextP,
    uint16_t instanceId,
    int *numDataP,
    lwm2m_data_t **dataArrayP,
    lwm2m_object_t *objectP)
{
    uint8_t result;
    int i;

    /* unused parameter */
    (void)contextP;

    // this is a single instance object
    if (instanceId != 0) {
        return COAP_404_NOT_FOUND;
    }

    // is the server asking for the full object ?
    if (*numDataP == 0) {
        uint16_t resList[] = { RES_M_SENSOR_VALUE, RES_O_UNITS };
        int nbRes          = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++) {
            (*dataArrayP)[i].id = resList[i];
        }
    }

    i = 0;
    do {
        switch ((*dataArrayP)[i].id) {
        case RES_M_SENSOR_VALUE:
            // printf("Reading humi, index = %d, dataArray id = %d\n", i, (*dataArrayP)[i].id);
            lwm2m_data_encode_float(sht30.RH, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;
        case RES_O_UNITS:
            lwm2m_data_encode_string(OBJECT_humi_UNIT, *dataArrayP + i);
            result = COAP_205_CONTENT;
            break;
        }

        i++;
    } while (i < *numDataP && result == COAP_205_CONTENT);

    return result;
}

static uint8_t prv_humi_discover(
    lwm2m_context_t *contextP,
    uint16_t instanceId,
    int *numDataP,
    lwm2m_data_t **dataArrayP,
    lwm2m_object_t *objectP)
{
    uint8_t result;
    int i;

    /* unused parameter */
    (void)contextP;

    // this is a single instance object
    if (instanceId != 0) {
        return COAP_404_NOT_FOUND;
    }

    result = COAP_205_CONTENT;

    // is the server asking for the full object ?
    if (*numDataP == 0) {
        uint16_t resList[] = { RES_M_SENSOR_VALUE, RES_O_UNITS };
        int nbRes          = sizeof(resList) / sizeof(uint16_t);

        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL)
            return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++) {
            (*dataArrayP)[i].id = resList[i];
        }
    } else {
        for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++) {
            switch ((*dataArrayP)[i].id) {
            case RES_M_SENSOR_VALUE:
            case RES_O_UNITS:
                break;
            default:
                result = COAP_404_NOT_FOUND;
            }
        }
    }

    return result;
}

lwm2m_object_t *get_object_humi(void)
{
    lwm2m_object_t *deviceObj;

    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj) {
        memset(deviceObj, 0, sizeof(lwm2m_object_t));

        deviceObj->versionMajor = 1;
        deviceObj->versionMinor = 1;

        deviceObj->objID        = 3304;
        deviceObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != deviceObj->instanceList) {
            memset(deviceObj->instanceList, 0, sizeof(lwm2m_list_t));
        } else {
            lwm2m_free(deviceObj);
            return NULL;
        }

        /*
         * And the private function that will access the object.
         * Those function will be called when a read/write/execute query is made by the server. In fact the library
         * don't need to know the resources of the object, only the server does.
         */
        deviceObj->readFunc     = prv_humi_read;
        deviceObj->discoverFunc = prv_humi_discover;
        deviceObj->writeFunc    = NULL;
        deviceObj->executeFunc  = NULL;
        deviceObj->userData     = NULL;
    }

    return deviceObj;
}
