#include "espnow_helper.h"

#include <string.h>

#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"


static const char *TAG = "ESPNOW";

void espnow_init(void) {
    ESP_ERROR_CHECK(nvs_flash_init());

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());

    ESP_LOGI(TAG, "ESP-NOW initialised");
}

void espnow_add_peer(const uint8_t *mac) {
    esp_now_peer_info_t peer = {0};

    memcpy(peer.peer_addr, mac, 6);

    peer.channel = 0;
    peer.ifidx = WIFI_IF_STA;
    peer.encrypt = false;

    ESP_ERROR_CHECK(esp_now_add_peer(&peer));
}

esp_err_t espnow_send(const uint8_t *mac, message_t *msg) {
    return esp_now_send(mac, (uint8_t *)msg, sizeof(message_t));
}

void espnow_register_receive(
    void (*callback)(const esp_now_recv_info_t *, const uint8_t *, int))
{
    esp_now_register_recv_cb(callback);
}