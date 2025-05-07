#include "wifi_handler.h"
#include "printer_helper.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
static int reconnect_counter = 0;

