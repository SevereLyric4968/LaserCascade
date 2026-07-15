#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <stdint.h>

#include "driver/gpio.h"


typedef enum {
    BOARD_MASTER,
    BOARD_SLAVE,
    BOARD_DMX
} board_type_t;

typedef struct{
    uint8_t id;

    board_type_t type;

    gpio_num_t statusLedPin;

    gpio_num_t laserRelayPin;

    uint8_t nextMac[6];
} board_config_t;

//CHANGE THIS FOR BOARD FLASHING!!!!
#define BOARD_ID 2

const board_config_t *board_config_get(void);

#endif