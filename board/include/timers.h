/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef IOTPROJECT_TIMERS_H
#define IOTPROJECT_TIMERS_H

#include <stdbool.h>

/**
 * Initialises the timers.
 *
 * @param heart_rate_handler handler to call every time the heart rate timer
 *                           expires
 * @param leds_handler handler to call every time the leds timer expires
 */
void timers_init(void *heart_rate_handler, void *leds_handler);

/**
 * Creates and starts the timer that simulates heart rate measurements.
 */
void timer_start_heart_rate();

/**
 * Creates and starts the timer that flickers the leds.
 */
void timer_start_leds();

#endif /* IOTPROJECT_TIMERS_H */
