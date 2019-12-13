/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef IOTPROJECT_RNG_H
#define IOTPROJECT_RNG_H

#include <unistd.h>

/**
 * Generates a random uint8 value.
 *
 * @return the random value
 */
uint8_t rng_generate_uint8();

#endif /* IOTPROJECT_RNG_H */
