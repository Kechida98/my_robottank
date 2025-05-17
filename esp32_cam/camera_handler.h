#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#include "esp_err.h"

esp_err_t camera_handler_init(void);
esp_err_t camera_capture_frame(void);
esp_err_t camera_handler_deinit(void);

#endif
