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


static void mqtt_event_handler_cb(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            PRINTFC_MQTT_HANDLER("MQTT connected");
            esp_mqtt_client_subscribe(client, "brandlarm/alarm", 0);
            break;

        case MQTT_EVENT_DATA: {
            PRINTFC_MQTT_HANDLER("Message received");

            char topic[64] = {0};
            memcpy(topic, event->topic, event->topic_len);
            topic[event->topic_len] = '\0';

            char payload[64] = {0};
            memcpy(payload, event->data, event->data_len);
            payload[event->data_len] = '\0';

            PRINTFC_MQTT_HANDLER("Topic: %s", topic);
            PRINTFC_MQTT_HANDLER("Payload: %s", payload);

            if (strcmp(topic, "brandlarm/alarm") == 0) {
                if (strstr(payload, "Brandrisk") != NULL) {
                    if (!alarm_active) {
                        PRINTFC_MQTT_HANDLER("🔥 Brandrisk upptäckt – buzzer startas");
                        alarm_active = true;
                        xTaskCreate(buzzer_task, "buzzer_task", 2048, NULL, 10, &buzzer_task_handle);
                    }
                } else if (strstr(payload, "Ingen brandrisk") != NULL) {
                    if (alarm_active) {
                        PRINTFC_MQTT_HANDLER("Ingen brandrisk – buzzer stoppas");
                        alarm_active = false;
                    }
                } else {
                    PRINTFC_MQTT_HANDLER("⚠️ Payload ignorerad (okänt tillstånd): %s", payload);
                }
            }
            break;
        }

        case MQTT_EVENT_ERROR:
            PRINTFC_MQTT_HANDLER("MQTT error occurred");
            break;

        default:
            PRINTFC_MQTT_HANDLER("MQTT event ID: %" PRId32, event_id);
            break;
    }
}