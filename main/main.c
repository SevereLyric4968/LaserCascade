#include "board_config.h"


void master_start(void);

void slave_start(void);

void dmx_start(void);

void app_main(void) {

    const board_config_t *board =
        board_config_get();

    if(board == NULL) {
        return;
    }

    switch(board->type) {

        case BOARD_MASTER:

            master_start();
            break;

        case BOARD_SLAVE:

            slave_start();
            break;

        case BOARD_DMX:

            dmx_start();
            break;
    }
}