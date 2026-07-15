#include "board_config.h"
#include "espnow_helper.h"
#include "hardware_helper.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static void receiveCallback(
    const esp_now_recv_info_t *info,
    const uint8_t *data,
    int len
)
{
    message_t msg;

    memcpy(&msg, data, sizeof(msg));

    printf("DMX board received: %s\n", msg.message);

    const board_config_t *board = board_config_get();

    status_set(STATUS_RX);

    laser_trigger(500);
}

void dmx_start(void) {
    const board_config_t *board = board_config_get();

    espnow_init();

    hardware_init();

    status_set(STATUS_READY);

    espnow_register_receive(receiveCallback);

    printf("DMX receiver ready\n");

    while (1) {
    hardware_update();

    vTaskDelay(pdMS_TO_TICKS(10));
    }
}