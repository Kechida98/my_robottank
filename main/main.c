#include "sdkconfig.h"

#include <stddef.h>
#include <string.h>

// BTstack related
#include <btstack_port_esp32.h>
#include <btstack_run_loop.h>
#include <btstack_stdio_esp32.h>

// Bluepad32 related
#include <arduino_platform.h>
#include <uni.h>

// Wi-Fi related
#include "wifi_handler.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"

static const char* TAG = "main";

// Autostart
#if CONFIG_AUTOSTART_ARDUINO
void initBluepad32() {
#else
int app_main(void) {
#endif  // !CONFIG_AUTOSTART_ARDUINO

// Don't use BTstack buffered UART. It conflicts with the console.
#ifndef CONFIG_ESP_CONSOLE_UART_NONE
#ifndef CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE
    btstack_stdio_init();
#endif
#endif

    // Configure BTstack for ESP32 VHCI Controller
    btstack_init();

    // hci_dump_init(hci_dump_embedded_stdout_get_instance());

    // Must be called before uni_init()
    uni_platform_set_custom(get_arduino_platform());

    // Init Bluepad32.
    uni_init(0 /* argc */, NULL /* argv */);

    // Does not return.
    btstack_run_loop_execute();
#if !CONFIG_AUTOSTART_ARDUINO
    return 0;
#endif  // !CONFIG_AUTOSTART_ARDUINO
}
