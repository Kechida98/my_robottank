#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"

// Event group bits for connection status
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_HAS_IP_BIT BIT1

// Maximum number of reconnect attempts
#define WIFI_RECONNECT_MAX_ATTEMPT 50

// Parameters required to initialize Wi-Fi
typedef struct {
    EventGroupHandle_t wifi_event_group; // Tracks connection status
    char ssid[32];                       // Wi-Fi SSID
    char password[64];                   // Wi-Fi password
} wifi_init_param_t;

// Starts the Wi-Fi handling logic
void wifi_handler_start(wifi_init_param_t *param);

#endif // WIFI_HANDLER_H
