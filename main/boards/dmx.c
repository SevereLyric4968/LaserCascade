#include "board_config.h"
#include "espnow_helper.h"
#include "hardware_helper.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"

#define DMX_UART UART_NUM_2
#define DMX_BAUD 250000

#define DMX_TX_PIN GPIO_NUM_17
#define DMX_EN_PIN GPIO_NUM_21


static uint8_t dmxData[513];
static TickType_t nextFrame = 0;

static void dmxInit(const board_config_t *board) {

    uart_config_t uartConfig = {
        .baud_rate = DMX_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    uart_driver_install(
        DMX_UART,
        1024,
        0,
        0,
        NULL,
        0
    );

    uart_param_config(
        DMX_UART,
        &uartConfig
    );

    uart_set_pin(
        DMX_UART,
        DMX_TX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    );

    memset(
        dmxData,
        0,
        sizeof(dmxData)
    );
    
    gpio_config_t enConfig = {
    .pin_bit_mask = (1ULL << DMX_EN_PIN),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&enConfig);

    gpio_set_level(DMX_EN_PIN, 1);
}

static void sendDmxFrame(void) {
    uart_wait_tx_done(
        DMX_UART,
        portMAX_DELAY
    );

    uart_set_baudrate(
        DMX_UART,
        90909
    );

    uint8_t zero = 0;

    uart_write_bytes(
        DMX_UART,
        &zero,
        1
    );

    uart_wait_tx_done(
        DMX_UART,
        portMAX_DELAY
    );

    uart_set_baudrate(
        DMX_UART,
        DMX_BAUD
    );

    uart_write_bytes(
        DMX_UART,
        (const char *)dmxData,
        sizeof(dmxData)
    );

    uart_wait_tx_done(
        DMX_UART,
        portMAX_DELAY
    );
}

static void dmxUpdate(void) {

    TickType_t now = xTaskGetTickCount();

    if(now >= nextFrame) {
        sendDmxFrame();

        nextFrame =
            now +
            pdMS_TO_TICKS(25);
    }
}

static void receiveCallback(
    const esp_now_recv_info_t *info,
    const uint8_t *data,
    int len
)
{
    message_t msg;

    memcpy(
        &msg,
        data,
        sizeof(msg)
    );

    printf("DMX board received: %s\n", msg.message);

    status_set(STATUS_RX);

    laser_trigger(500);
}

void dmx_start(void) {

    const board_config_t *board =
        board_config_get();

    espnow_init();

    hardware_init();

    dmxInit(board);

    espnow_register_receive(receiveCallback);

    status_set(STATUS_READY);

    while(1) {
        hardware_update();

        if(!laser_is_active()) {
            dmxData[1] = 0;
        }
        else{
            dmxData[1] = 255;
        }

        dmxUpdate();

        vTaskDelay(
            pdMS_TO_TICKS(1)
        );
    }
}