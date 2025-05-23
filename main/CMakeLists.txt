set(srcs
    "main.c"
    "wifi_handler.c"
    "sketch.cpp"
    "mqtt_handler.c" 
)

set(requires
    bluepad32
    bluepad32_arduino
    arduino
    btstack
    esp_wifi
    esp_netif
    esp_event
    nvs_flash
    mqtt             
    ESP32Servo)

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS "."
    REQUIRES ${requires}
)
