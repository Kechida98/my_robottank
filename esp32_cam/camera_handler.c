#include "camera_handler.h"
#include "esp_camera.h"
#include "printer_helper.h"

esp_err_t camera_handler_init(void) {
    camera_config_t config = {
    .pin_pwdn  = 32,
    .pin_reset = -1,
    .pin_xclk = 0,
    .pin_sscb_sda = 26,
    .pin_sscb_scl = 27,

    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 21,
    .pin_d2 = 19,
    .pin_d1 = 18,
    .pin_d0 = 5,

    .pin_vsync = 25,
    .pin_href  = 23,
    .pin_pclk  = 22,

    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG,
    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 10,
    .fb_count = 2
};



    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        PRINTFC_CAMERA("Camera init failed: 0x%x", err);
    }
    return err;
}

esp_err_t camera_capture_frame(void) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) return ESP_FAIL;
    esp_camera_fb_return(fb);
    return ESP_OK;
}

esp_err_t camera_handler_deinit(void) {
    return esp_camera_deinit();
}
