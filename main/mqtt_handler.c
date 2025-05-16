#include "mqtt_handler.h"
#include "mqtt_client.h"
#include "printer_helper.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include <inttypes.h>  

#define BUZZER_GPIO GPIO_NUM_4

static TaskHandle_t buzzer_task_handle = NULL;
static bool alarm_active = false;

static void buzzer_beep(int duration_ms) {
    gpio_set_level(BUZZER_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(duration_ms));
    gpio_set_level(BUZZER_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(150));  
}

static void buzzer_task(void *pvParameters) {
    while (alarm_active) {
        // S = ...
        for (int i = 0; i < 3 && alarm_active; i++) {
            buzzer_beep(150);  // dot
        }

        vTaskDelay(pdMS_TO_TICKS(300));

        // O = ---
        for (int i = 0; i < 3 && alarm_active; i++) {
            buzzer_beep(450);  // dash
        }

        vTaskDelay(pdMS_TO_TICKS(300));  

        // S = ...
        for (int i = 0; i < 3 && alarm_active; i++) {
            buzzer_beep(150);  // dot
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }

    vTaskDelete(NULL);
}