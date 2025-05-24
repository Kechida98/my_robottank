#ifndef CAMERA_HANDLER_H
#define CAMERA_HANDLER_H

#include "esp_err.h"

// Initializes the camera (sets up hardware, config, etc.)
esp_err_t camera_handler_init(void);

// Captures a single frame from the camera
esp_err_t camera_capture_frame(void);

// Deinitializes and releases camera resources
esp_err_t camera_handler_deinit(void);

#endif
