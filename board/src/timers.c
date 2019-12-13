/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "timers.h"

#include "app_error.h"
#include "app_timer.h"

APP_TIMER_DEF(heart_rate_timer);
APP_TIMER_DEF(leds_timer);

void timers_init(void *heart_rate_handler, void *leds_handler) {
    // Initialise the timers.
    ret_code_t result = app_timer_init();
    APP_ERROR_CHECK(result);

    // Create the heart rate timer.
    result = app_timer_create(&heart_rate_timer, APP_TIMER_MODE_REPEATED,
                              heart_rate_handler);

    // Create the leds timer.
    result = app_timer_create(&leds_timer, APP_TIMER_MODE_REPEATED,
                              leds_handler);
    APP_ERROR_CHECK(result);
}

void timer_start_heart_rate() {
    const ret_code_t result = app_timer_start(heart_rate_timer,
                                              APP_TIMER_TICKS(1000),
                                              NULL);
    APP_ERROR_CHECK(result);
}

void timer_start_leds() {
    const ret_code_t result = app_timer_start(leds_timer,
                                              APP_TIMER_TICKS(250),
                                              NULL);
    APP_ERROR_CHECK(result);
}