
#ifndef LWM2MCLIENT_H_
#define LWM2MCLIENT_H_

#include "Arduino.h"
#include "Microfire_SHT3x.h"
#include "liblwm2m.h"

/*
 * object_device.c
 */
lwm2m_object_t *get_object_device(void);
// void free_object_device(lwm2m_object_t *objectP);
// uint8_t device_change(lwm2m_data_t *dataArray, lwm2m_object_t *objectP);
// void display_device_object(lwm2m_object_t *objectP);

// /*
//  * object_firmware.c
//  */
// lwm2m_object_t *get_object_firmware(void);
// void free_object_firmware(lwm2m_object_t *objectP);
// void display_firmware_object(lwm2m_object_t *objectP);
// /*
//  * object_location.c
//  */
// lwm2m_object_t *get_object_location(void);
// void free_object_location(lwm2m_object_t *object);
// void display_location_object(lwm2m_object_t *objectP);
// /*
//  * object_test.c
//  */
// #define TEST_OBJECT_ID 31024
// lwm2m_object_t *get_test_object(void);
// void free_test_object(lwm2m_object_t *object);
// void display_test_object(lwm2m_object_t *objectP);

/*
 * object_server.c
 */
lwm2m_object_t *get_server_object(int serverId, const char *binding, int lifetime, bool storing);
// void clean_server_object(lwm2m_object_t *object);
// void display_server_object(lwm2m_object_t *objectP);
// void copy_server_object(lwm2m_object_t *objectDest, lwm2m_object_t *objectSrc);

// /*
//  * object_connectivity_moni.c
//  */
// lwm2m_object_t *get_object_conn_m(void);
// void free_object_conn_m(lwm2m_object_t *objectP);
// uint8_t connectivity_moni_change(lwm2m_data_t *dataArray, lwm2m_object_t *objectP);
//
// /*
//  * object_connectivity_stat.c
//  */
// extern lwm2m_object_t *get_object_conn_s(void);
// void free_object_conn_s(lwm2m_object_t *objectP);
// extern void conn_s_updateTxStatistic(lwm2m_object_t *objectP, uint16_t txDataByte, bool smsBased);
// extern void conn_s_updateRxStatistic(lwm2m_object_t *objectP, uint16_t rxDataByte, bool smsBased);
//
// /*
//  * object_access_control.c
//  */
// lwm2m_object_t *acc_ctrl_create_object(void);
// void acl_ctrl_free_object(lwm2m_object_t *objectP);
// bool acc_ctrl_obj_add_inst(
//     lwm2m_object_t *accCtrlObjP,
//     uint16_t instId,
//     uint16_t acObjectId,
//     uint16_t acObjInstId,
//     uint16_t acOwner);
// bool acc_ctrl_oi_add_ac_val(lwm2m_object_t *accCtrlObjP, uint16_t instId, uint16_t aclResId, uint16_t acValue);
// /*
//  * lwm2mclient.c
//  */
// void handle_value_changed(lwm2m_context_t *lwm2mH, lwm2m_uri_t *uri, const char *value, size_t valueLength);
// /*
//  * system_api.c
//  */
// void init_value_change(lwm2m_context_t *lwm2m);
// void system_reboot(void);

/*
 * object_security.c
 */
lwm2m_object_t *get_security_object(
    int serverId,
    const char *serverUri,
    char *bsPskId,
    char *psk,
    uint16_t pskLen,
    bool isBootstrap);
// void clean_security_object(lwm2m_object_t *objectP);
// char *get_server_uri(lwm2m_object_t *objectP, uint16_t secObjInstID);
// void display_security_object(lwm2m_object_t *objectP);
// void copy_security_object(lwm2m_object_t *objectDest, lwm2m_object_t *objectSrc);

/*
 * object_temp.c
 */

lwm2m_object_t *get_object_temp(void);
lwm2m_object_t *get_object_humi(void);
void update_temp(lwm2m_context_t *contextP);
void update_humi(lwm2m_context_t *contextP);

void output_buffer(FILE *stream, const uint8_t *buffer, size_t length, int indent);

#endif /* LWM2MCLIENT_H_ */
