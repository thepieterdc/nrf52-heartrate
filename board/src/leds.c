/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "leds.h"

#include "boards.h"

void leds_init() {
    bsp_board_init(BSP_INIT_LEDS);
}

void leds_set_all(bool enable) {
    if (enable) {
        // Enable the leds.
        bsp_board_leds_on();
    } else {
        // Disable the leds.
        bsp_board_leds_off();
    }
}
