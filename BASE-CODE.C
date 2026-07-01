#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "driver/cam_ctlr.h"
#include "esp_hosted_api.h"

#define WIFI_NAME "FBI SURVEILLANCE VAN"
#define WIFI_PW   "fbiopenup"
#define BOUND     "----BOUND----"
#define HDR_STR   "--" BOUND "\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n"
#define IP_OK     BIT0
#define IP_FAIL   BIT1

static const char *my_tag = "cam_app";
static EventGroupHandle_t evt_grp;
static httpd_handle_t srv = NULL;
static cam_ctlr_handle_t cam_h = NULL;

static void got_ip(void *arg, esp_event_base_t base, int32_t id, void *data)
{
    if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *ev = data;
        ESP_LOGI(my_tag, "ip: " IPSTR, IP2STR(&ev->ip_info.ip));
        xEventGroupSetBits(evt_grp, IP_OK);
    }
}

static esp_err_t send_stream(httpd_req_t *r)
{
    esp_err_t rv;
    char chunk[128];
    cam_frame_t jpeg;

    rv = httpd_resp_set_type(r, "multipart/x-mixed-replace;boundary=" BOUND);
    if (rv != ESP_OK) return rv;

    httpd_resp_set_hdr(r, "Cache-Control", "no-cache");
    httpd_resp_set_hdr(r, "X-Accel-Buffering", "no");

    while (1) {
        if (cam_ctlr_receive_frame(&jpeg, 1000 / portTICK_PERIOD_MS) != ESP_OK) {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            continue;
        }

        int len = snprintf(chunk, 128, HDR_STR, jpeg.data_len);
        if (len < 0 || len >= 128) {
            cam_ctlr_return_frame(&jpeg);
            break;
        }

        rv = httpd_resp_send_chunk(r, chunk, len);
        if (rv == ESP_OK) {
            rv = httpd_resp_send_chunk(r, (char *)jpeg.data, jpeg.data_len);
        }
        if (rv == ESP_OK) {
            rv = httpd_resp_send_chunk(r, "\r\n", 2);
        }

        cam_ctlr_return_frame(&jpeg);

        if (rv != ESP_OK) break;
    }

    if (rv == ESP_OK) {
        httpd_resp_send_chunk(r, NULL, 0);
    }

    return rv;
}

static esp_err_t send_page(httpd_req_t *r)
{
    const char *html = "<html><head><title>My Cam</title></head>"
        "<body style='background:#111;text-align:center;'>"
        "<h2 style='color:#eee;font-family:arial;'>ESP32-P4 + C6 Stream</h2>"
        "<img src='/stream' style='max-width:95%;border:1px solid #555;' />"
        "</body></html>";

    httpd_resp_set_type(r, "text/html");
    return httpd_resp_send(r, html, HTTPD_RESP_USE_UNKNOWN_LEN);
}

static void start_srv(void)
{
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    cfg.server_port = 80;
    cfg.stack_size = 8192;
    cfg.max_open_sockets = 4;

    if (httpd_start(&srv, &cfg) != ESP_OK) {
        ESP_LOGE(my_tag, "server failed");
        return;
    }

    httpd_uri_t u1 = { .uri = "/", .method = HTTP_GET, .handler = send_page };
    httpd_uri_t u2 = { .uri = "/stream", .method = HTTP_GET, .handler = send_stream };

    httpd_register_uri_handler(srv, &u1);
    httpd_register_uri_handler(srv, &u2);
}

static void init_cam(void)
{
    cam_ctlr_config_t c;
    c.data_width = 8;
    c.mipi_csi_lane_num = 2;
    c.byte_swap_en = false;
    c.queue_items = 4;
    c.buffer_size = 900000;

    ESP_ERROR_CHECK(cam_new_ctlr(&c, &cam_h));
    ESP_ERROR_CHECK(cam_ctlr_enable(cam_h));
    ESP_ERROR_CHECK(cam_ctlr_start(cam_h));
}

static void init_wifi(void)
{
    evt_grp = xEventGroupCreate();

    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &got_ip, NULL);

    esp_hosted_config_t hcfg = ESP_HOSTED_CONFIG_DEFAULT();
    esp_netif_t *n = esp_hosted_init(&hcfg);

    if (n == NULL) {
        ESP_LOGE(my_tag, "c6 bridge failed");
        return;
    }

    esp_hosted_wifi_config_t wcfg;
    memset(&wcfg, 0, sizeof(wcfg));
    strncpy((char *)wcfg.sta.ssid, WIFI_NAME, sizeof(wcfg.sta.ssid));
    strncpy((char *)wcfg.sta.password, WIFI_PW, sizeof(wcfg.sta.password));

    ESP_ERROR_CHECK(esp_hosted_wifi_set_config(ESP_HOSTED_WIFI_MODE_STA, &wcfg));
    ESP_ERROR_CHECK(esp_hosted_wifi_connect());

    EventBits_t bits = xEventGroupWaitBits(evt_grp, IP_OK | IP_FAIL, 0, 0, 30000 / portTICK_PERIOD_MS);

    if (bits & IP_OK) {
        ESP_LOGI(my_tag, "wifi ok: %s", WIFI_NAME);
    } else {
        ESP_LOGE(my_tag, "wifi timeout");
    }
}

void app_main(void)
{
    esp_err_t e = nvs_flash_init();
    if (e == ESP_ERR_NVS_NO_FREE_PAGES || e == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        e = nvs_flash_init();
    }
    ESP_ERROR_CHECK(e);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    init_wifi();
    init_cam();
    start_srv();
}
