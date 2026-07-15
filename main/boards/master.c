#include "board_config.h"
#include "espnow_helper.h"
#include "hardware_helper.h"

#include "driver/gpio.h"

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN GPIO_NUM_18


void master_start(void) {
    const board_config_t *board = board_config_get();

    espnow_init();

    espnow_add_peer(board->nextMac);

    hardware_init();

    gpio_input_init(BUTTON_PIN);

    message_t msg;

    strcpy(msg.message, "Laser trigger");

    status_set(STATUS_READY);

    while(1) {
        if(gpio_get_level(BUTTON_PIN)) {
            printf("Button pressed\n");

            esp_err_t result = espnow_send(board->nextMac, &msg);

            status_set(STATUS_TX);

            laser_trigger(500);

            if(result == ESP_OK) {
                printf("Trigger sent\n");
            }
            else {
                printf("Send failed\n");
            }

            // wait for release
            while(gpio_get_level(BUTTON_PIN)) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }

        hardware_update();

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}