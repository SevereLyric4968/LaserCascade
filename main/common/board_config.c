#include "board_config.h"


static const board_config_t boardConfigs[] ={
    {
        .id = 1,
        .type = BOARD_MASTER,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0x08,0xB6,0x1F,0x28,0x86,0xE8
        }
    },

    {
        .id = 2,
        .type = BOARD_SLAVE,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0x08,0xB6,0x1F,0x29,0x8E,0xF8
        }
    },

    {
        .id = 3,
        .type = BOARD_DMX,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0,0,0,0,0,0
        }
    }
};

const board_config_t *board_config_get(void) {
    for(int i = 0; i < sizeof(boardConfigs)/sizeof(boardConfigs[0]); i++)
    {
        if(boardConfigs[i].id == BOARD_ID)
        {
            return &boardConfigs[i];
        }
    }

    return NULL;
}