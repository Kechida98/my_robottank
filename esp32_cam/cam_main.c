#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "wifi_handler.h"
#include "camera_handler.h"
#include "web_server.h"
#include "printer_helper.h"
#include <string.h>


void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    static wifi_init_param_t wifi_params = {0}; 

    wifi_params.wifi_event_group = xEventGroupCreate();

    strcpy(wifi_params.ssid, "");
    strcpy(wifi_params.password, "");

    wifi_handler_start(&wifi_params);

    if (camera_handler_init() == ESP_OK) {
        start_web_server();
    } else {
        PRINTFC_MAIN("Failed to initialize the camera");
    }
}
