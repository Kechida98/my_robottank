#include "wifi_handler.h"
#include "printer_helper.h"
#include "esp_system.h"
#include "esp_wifi.h"

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
            reconnect_counter = 0; 
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

static void ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    wifi_init_param_t *param = (wifi_init_param_t *)arg;
    switch (event_id) {
        case IP_EVENT_STA_GOT_IP:
            PRINTFC_WIFI_HANDLER("Got IP address");
            xEventGroupSetBits(param->wifi_event_group, WIFI_HAS_IP_BIT);
            break;

        default:
            break;
    }
}

void wifi_handler_start(wifi_init_param_t *param) {
    PRINTFC_WIFI_HANDLER("WiFi Handler is starting");

    // Kontrollera att SSID och lösenord är tillgängliga
    if (strlen(param->ssid) == 0 || strlen(param->password) == 0) {
        PRINTFC_WIFI_HANDLER("SSID or password is missing!");
        return;
    }

    PRINTFC_WIFI_HANDLER("Using SSID: %s%s%s", green, param->ssid, reset);
    PRINTFC_WIFI_HANDLER("Using password: %s%s%s", green, param->password, reset);

    // Initiera nätverksgränssnitt
    PRINTFC_WIFI_HANDLER("Initializing network interface");
    ESP_ERROR_CHECK(esp_netif_init());

    esp_netif_t *netif = esp_netif_create_default_wifi_sta();
    assert(netif);

    // Initiera Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Registrera eventhanterare
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, param, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, param, NULL));

    // Ställ in Wi-Fi-konfiguration
    wifi_config_t wifi_config = {0};
    strncpy((char *)wifi_config.sta.ssid, param->ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, param->password, sizeof(wifi_config.sta.password));
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    PRINTFC_WIFI_HANDLER("WiFi is starting...");

    // Vänta på anslutning
    PRINTFC_WIFI_HANDLER("Waiting for Wi-Fi connection...");
    EventBits_t bits = xEventGroupWaitBits(param->wifi_event_group,
                                           WIFI_HAS_IP_BIT,
                                           pdFALSE,
                                           pdTRUE,
                                           pdMS_TO_TICKS(30000));

    if (bits & WIFI_HAS_IP_BIT) {
        PRINTFC_WIFI_HANDLER("Successfully connected to Wi-Fi.");
    } else {
        PRINTFC_WIFI_HANDLER("Failed to connect to Wi-Fi within timeout.");
    }
}
