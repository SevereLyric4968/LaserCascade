#ifndef ESPNOW_HELPER_H
#define ESPNOW_HELPER_H

#include "esp_now.h"


typedef struct {
    char message[32];
} message_t;

void espnow_init(void);

void espnow_add_peer(const uint8_t *mac);

esp_err_t espnow_send(const uint8_t *mac, message_t *msg);

void espnow_register_receive(
    void (*callback)
    (
        const esp_now_recv_info_t *,
        const uint8_t *,
        int
    )
);

#endif