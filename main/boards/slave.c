#include "board_config.h"
#include "espnow_helper.h"
#include "hardware_helper.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


void receiveCallback(
    const esp_now_recv_info_t *info,
    const uint8_t *data,
    int len
)
{
    printf("message received");
    message_t msg;

    memcpy( &msg, data, sizeof(msg));

    const board_config_t *board = board_config_get();

    espnow_send(board->nextMac, &msg);

    status_set(STATUS_RX);

    laser_trigger(500);
}

void slave_start(void) {
    const board_config_t *board = board_config_get();

    espnow_init();

    espnow_add_peer(board->nextMac);

    hardware_init();

    espnow_register_receive(receiveCallback);

    status_set(STATUS_READY);

    while (1) {
    hardware_update();

    vTaskDelay(pdMS_TO_TICKS(10));
    }
}