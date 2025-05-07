#include "wifi_handler.h"
#include "printer_helper.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
static int reconnect_counter = 0;

static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    wifi_init_param_t *param = (wifi_init_param_t *)arg;
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            PRINTFC_WIFI_HANDLER("WiFi started, attempting to connect...");
            esp_wifi_connect();
            break;

        case WIFI_EVENT_STA_CONNECTED:
            PRINTFC_WIFI_HANDLER("WiFi connected");
            reconnect_counter = 0; // Reset counter if connection sucssesful
            xEventGroupSetBits(param->wifi_event_group, WIFI_CONNECTED_BIT);
            break;

        case WIFI_EVENT_STA_DISCONNECTED:
            PRINTFC_WIFI_HANDLER("WiFi disconnected");
            xEventGroupClearBits(param->wifi_event_group, WIFI_CONNECTED_BIT | WIFI_HAS_IP_BIT);
            if (reconnect_counter < WIFI_RECONNECT_MAX_ATTEMPT) {
                reconnect_counter++;
                PRINTFC_WIFI_HANDLER("Reconnecting (%d/%d)...", reconnect_counter, WIFI_RECONNECT_MAX_ATTEMPT);
                esp_wifi_connect();
            } else {
                PRINTFC_WIFI_HANDLER("Reached maximum reconnection attempts.");
            }
            break;

        default:
            break;
    }
}

