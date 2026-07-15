#include "hardware_helper.h"
#include "board_config.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static gpio_num_t statusLedPin;
static gpio_num_t relayPin;

static bool relayActive = false;
static TickType_t relayEndTick;

static status_pattern_t currentPattern = STATUS_READY;

static TickType_t nextLedTick = 0;

static bool ledState = false;

static uint8_t flashCount = 0;

void hardware_init(void) {
    const board_config_t *board = board_config_get();

    statusLedPin = board->statusLedPin;

    relayPin = board->laserRelayPin;

    gpio_config_t config = {
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
    };

    config.pin_bit_mask = 1ULL << statusLedPin;
    gpio_config(&config);

    gpio_set_level(statusLedPin,0);
    gpio_set_level(relayPin,0);
}

void gpio_input_init(gpio_num_t pin) {
    gpio_config_t config =
    {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);
}

void laser_trigger(uint32_t durationMs) {
    gpio_set_level(
        relayPin,
        1
    );

    relayEndTick = xTaskGetTickCount() + pdMS_TO_TICKS(durationMs);

    relayActive = true;
}

void hardware_update(void) {
    if(relayActive)
    {
        if(xTaskGetTickCount() >= relayEndTick)
        {
            gpio_set_level(
                relayPin,
                0
            );

            relayActive = false;
        }
    }
    TickType_t now = xTaskGetTickCount();

    if(now < nextLedTick){
        return;
    }

    switch(currentPattern) {
    
        case STATUS_READY:

            ledState = !ledState;

            gpio_set_level(statusLedPin, ledState);

            if(ledState) {
                nextLedTick = now + pdMS_TO_TICKS(100);
            }
            else {
                nextLedTick = now + pdMS_TO_TICKS(900);
            }

            break;

        case STATUS_RX:

            ledState = !ledState;

            gpio_set_level(statusLedPin, ledState);

            flashCount--;

            nextLedTick = now + pdMS_TO_TICKS(100);

            if(flashCount == 0) {
                currentPattern = STATUS_READY;
            }

            break;

        case STATUS_TX:

            ledState = !ledState;

            gpio_set_level(statusLedPin, ledState);

            flashCount--;

            nextLedTick = now + pdMS_TO_TICKS(100);

            if(flashCount == 0)
            {
                currentPattern = STATUS_READY;
            }

            break;

        case STATUS_ERROR:

            ledState = !ledState;

            gpio_set_level(statusLedPin, ledState);

            nextLedTick = now + pdMS_TO_TICKS(200);

            break;

        case STATUS_OFF:

            gpio_set_level(statusLedPin,0);

            break;
    }
}

void status_set(status_pattern_t pattern) {
    currentPattern = pattern;

    ledState = false;
    gpio_set_level(statusLedPin, 0);

    nextLedTick = xTaskGetTickCount();

    switch(pattern)
    {
        case STATUS_TX:
            flashCount = 4;
            break;

        case STATUS_RX:
            flashCount = 2;
            break;

        default:
            flashCount = 0;
            break;
    }
}