#include "sdkconfig.h"
#include <stddef.h>
#include <string.h>

//BTstack / Bluepad32
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <btstack_stdio_esp32.h>
#include <arduino_platform.h>
#include <uni.h>

//Wi-Fi och MQTT
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "printer_helper.h"  

#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//MQTT-task 
void mqtt_task(void* arg) {
    PRINTFC_MQTT_HANDLER("🚀 Starting MQTT task...");
    mqtt_app_start();
    vTaskDelete(NULL); //MQTT is asyncronized
}

//Bluepad32-task
void bluepad32_task(void* arg) {
    PRINTFC_MAIN("🎮 Starting Bluepad32 task...");
    btstack_init();
    uni_platform_set_custom(get_arduino_platform());
    uni_init(0, NULL);
    btstack_run_loop_execute(); //Blocker
    vTaskDelete(NULL);
}

#if CONFIG_AUTOSTART_ARDUINO
void initBluepad32() {
#else
int app_main(void) {
#endif

#ifndef CONFIG_ESP_CONSOLE_UART_NONE
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif
#endif

    //NVS-init
    PRINTFC_MAIN("📦 Initializing NVS...");
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    //Network + Event loop
    PRINTFC_MAIN("🌐 Initializing network interface...");
    ESP_ERROR_CHECK(esp_netif_init());

    PRINTFC_MAIN("🔁 Creating default event loop...");
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    //Wi-Fi startup
    PRINTFC_WIFI_HANDLER("📡 Starting Wi-Fi...");
    EventGroupHandle_t wifi_event_group = xEventGroupCreate();
    wifi_init_param_t w_param = {
        .ssid = CONFIG_WIFI_SSID,
        .password = CONFIG_WIFI_PASSWORD,
        .wifi_event_group = wifi_event_group,
    };
    wifi_handler_start(&w_param);

    //MQTT on core 0
    xTaskCreatePinnedToCore(
        mqtt_task,
        "mqtt_task",
        8192,
        NULL,
        5,
        NULL,
        0
    );

    //Bluepad32 on core 1
    xTaskCreatePinnedToCore(
        bluepad32_task,
        "bluepad32_task",
        8192,
        NULL,
        5,
        NULL,
        1
    );

#if !CONFIG_AUTOSTART_ARDUINO
    return 0;
#endif
}