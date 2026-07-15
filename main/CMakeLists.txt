idf_component_register(

    SRCS

        "main.c"

        "boards/master.c"
        "boards/slave.c"
        "boards/dmx.c"

        "common/board_config.c"
        "common/espnow_helper.c"
        "common/hardware_helper.c"


    INCLUDE_DIRS

        "."
        "include"
)