#ifndef __DEFINITIONS__
#define __DEFINITIONS__

//Sizes

#define MAX_SOCKET_BUFF_SIZE 1024u
#define MAX_STR_LEN 64u

//WIFI

#define WIFI_TIMEOUT 8u

#define WIFI_CONFIG_REQUEST_HEADER 1u
#define SUCCESS_RESPONSE_HEADER 0u
#define WIFI_CONFIG_SUCCESS_RESPONSE_HEADER 1u
#define INVALID_WIFI_CONFIG_RESPONSE_HEADER 1u
#define WIFI_CONNECTION_FAILED_RESPONSE_HEADER 2u

#define WIFI_CONFIG_SSID_KEY ("wifi_ssid")
#define WIFI_CONFIG_PWD_KEY ("wifi_pwd")
#endif
