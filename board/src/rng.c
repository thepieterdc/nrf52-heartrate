/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "rng.h"

#include "app_error.h"
#include "nrf_soc.h"

#define CACHE_SIZE 32

static uint8_t cache[CACHE_SIZE] = {60};
static size_t cache_idx = 0;

uint8_t rng_generate_uint8() {
    // Find out whether random data is available in the pool.
    uint8_t available;
    sd_rand_application_bytes_available_get(&available);

    if (available > 0) {
        // Generate a new random number and save it in the cache.
        const ret_code_t result = sd_rand_application_vector_get(cache + cache_idx, 1);
        APP_ERROR_CHECK(result);
    }

    // Return a number from the cache.
    const uint8_t ret = cache[cache_idx];
    cache_idx = (cache_idx + 1) % CACHE_SIZE;
    return ret;
}