#include "board_config.h"


static const board_config_t boardConfigs[] ={
    {
        .id = 1,
        .type = BOARD_MASTER,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0x20,0x9B,0xA9,0x67,0xC1,0x90
        },
        .cascadeDelayMs = 100,
        .laserOnMs = 500
    },

    {
        .id = 2,
        .type = BOARD_SLAVE,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0x68,0x09,0x47,0x9D,0x4A,0xB0
        },
        .cascadeDelayMs = 100,
        .laserOnMs = 500
        
    },

    {
        .id = 3,
        .type = BOARD_SLAVE,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0x68,0x09,0x47,0x9D,0x63,0x90
        },
        .cascadeDelayMs = 100,
        .laserOnMs = 500
    },

    {
        .id = 4,
        .type = BOARD_DMX,
        .statusLedPin = GPIO_NUM_19,
        .laserRelayPin = GPIO_NUM_23,
        .nextMac =
        {
            0,0,0,0,0,0
        },
        .cascadeDelayMs = 0,
        .laserOnMs = 3000
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