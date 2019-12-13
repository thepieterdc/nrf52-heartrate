/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef IOTPROJECT_LEDS_H
#define IOTPROJECT_LEDS_H

#include <stdbool.h>

/**
 * Initialises the leds.
 */
void leds_init();

/**
 * Sets the status of all 4 leds.
 *
 * @param enable true to enable the leds, false to disable
 */
void leds_set_all(bool enable);

#endif /* IOTPROJECT_LEDS_H */
