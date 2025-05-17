#include "web_server.h"
#include "esp_http_server.h"
#include "esp_camera.h"
#include "printer_helper.h"

static const char index_html[] =
"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\" />"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />"
"<title>ESP32-CAM Live Stream</title><style>"
"body{background:#121212;color:#f5f5f5;font-family:sans-serif;text-align:center;margin:0}"
"h1{color:#90caf9;margin:20px;font-weight:400}"
".stream-box{max-width:90%;border:3px solid #90caf9;border-radius:8px;"
"box-shadow:0 0 12px rgba(0,0,0,0.6);overflow:hidden;margin:auto}"
"img{width:100%;display:block}"
"</style></head><body><h1>📷 ESP32-CAM Live Stream</h1>"
"<div class=\"stream-box\"><img src=\"/stream\" alt=\"ESP32 Stream\" /></div></body></html>";

static esp_err_t index_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
}

static esp_err_t stream_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");

    while (true) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) continue;

        char header[64];
        int hlen = snprintf(header, sizeof(header),
                            "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);

        if (httpd_resp_send_chunk(req, header, hlen) != ESP_OK ||
            httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len) != ESP_OK) {
            esp_camera_fb_return(fb);
            break;
        }

        esp_camera_fb_return(fb);
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

void start_web_server(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = index_handler,
            .user_ctx = NULL
        };

        httpd_uri_t stream_uri = {
            .uri = "/stream",
            .method = HTTP_GET,
            .handler = stream_handler,
            .user_ctx = NULL
        };

        esp_err_t err_root = httpd_register_uri_handler(server, &root_uri);
        esp_err_t err_stream = httpd_register_uri_handler(server, &stream_uri);

        if (err_root == ESP_OK) {
            PRINTFC_SERVER("✅ Root handler registered at /");
        } else {
            PRINTFC_SERVER("❌ Failed to register / handler: %d", err_root);
        }

        if (err_stream == ESP_OK) {
            PRINTFC_SERVER("✅ Stream handler registered at /stream");
        } else {
            PRINTFC_SERVER("❌ Failed to register /stream handler: %d", err_stream);
        }
    } else {
        PRINTFC_SERVER("❌ Failed to start HTTP server");
    }
}

