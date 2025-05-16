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
