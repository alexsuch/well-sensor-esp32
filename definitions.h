#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#define MAX_STR_LEN                                 64u
#define WIFI_TIMEOUT                                8u

// TCP Request headers
#define SET_WIFI_CONFIG_REQUEST_HEADER              1u
#define GET_IP_REQUEST_HEADER                       2u
#define SET_CONFIG_REQUEST_HEADER                   3u

// TCP Response headers
#define INVALID_WIFI_CONFIG_RESPONSE_HEADER         1u
#define WIFI_CONNECTION_FAILED_RESPONSE_HEADER      2u
#define SAVE_WIFI_FAILED_RESPONSE_HEADER            3u
#define SAVE_MQTT_FAILED_RESPONSE_HEADER            4u
#define MQTT_CONNECTION_FAILED_RESPONSE_HEADER      5u
#define INVALID_MQTT_CONFIG_RESPONSE_HEADER         6u

// WIFI Config Keys
#define WIFI_CONFIG_SSID                            ("ssid")
#define WIFI_CONFIG_PWD                             ("ssidPwd")

// MQTT Config Keys
#define MQTT_CONFIG_SERVER                          ("server")
#define MQTT_CONFIG_PORT                            ("port")
#define MQTT_CONFIG_USER                            ("user")
#define MQTT_CONFIG_PWD                             ("mqttPwd")

#endif
