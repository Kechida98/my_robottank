idf_component_register(
    SRCS "main.c" "wifi_handler.c" "camera_handler.c" "web_server.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event esp_http_server nvs_flash esp32-camera
)
