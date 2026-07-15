#ifndef HARDWARE_HELPER_H
#define HARDWARE_HELPER_H

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"

typedef enum {
    STATUS_OFF,
    STATUS_READY,
    STATUS_TX,
    STATUS_RX,
    STATUS_ERROR
} status_pattern_t;

void hardware_init(void);

void gpio_input_init(gpio_num_t pin);

void hardware_update(void);

void laser_trigger(uint32_t durationMs);

bool laser_is_active(void);

void status_set(status_pattern_t pattern);

#endif