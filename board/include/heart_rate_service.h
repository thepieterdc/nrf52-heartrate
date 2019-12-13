/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef IOTPROJECT_HEART_RATE_SERVICE_H
#define IOTPROJECT_HEART_RATE_SERVICE_H

#include "ble.h"
#include "ble_gatts.h"

#define IOTPROJECT_UUID {{0x21, 0x08, 0x97, 0x00, 0x04, 0x05, 0x97, 0x00, 0x27, 0x12, 0x96, 0x00, 0x00, 0x00, 0x00, 0x00}}
#define HEART_RATE_MEASUREMENT_SERVICE 0xAAAA
#define HEART_RATE_MEASUREMENT_CHARACTERISTIC 0xAAAB
#define HEART_RATE_BOUNDS_SERVICE 0xAABA
#define HEART_RATE_BOUNDS_CHARACTERISTIC 0xAABB

#define HEART_RATE_MAX 200
#define HEART_RATE_MIN 30

/**
 * Heart rate service configuration.
 */
typedef struct heart_rate_srv {
    uint16_t conn_handle;

    uint16_t heart_rate_measurement_srv_handle;
    uint16_t heart_rate_bounds_srv_handle;

    ble_gatts_char_handles_t heart_rate_measurement_handles;
    ble_gatts_char_handles_t heart_rate_bound_handles;

    uint8_t max;
    uint8_t min;
    uint8_t previous;
} heart_rate_srv_t;

/**
 * Event handler for a heart rate service.
 *
 * @param event the event
 * @param hrs_instance the heart rate service instance
 */
void heart_rate_srv_event(const ble_evt_t *event, void *hrs_instance);

/**
 * Initialises the heart rate service.
 *
 * @param instance the instance to initialise
 */
void heart_rate_srv_init(heart_rate_srv_t *instance);

/**
 * Generates a random heart rate value, with respect to the bounds configured in
 * the service.
 */
uint8_t heart_rate_srv_random(heart_rate_srv_t *instance);

/**
 * Updates the heart rate measurement.
 *
 * @param srv heart rate service
 * @param value the new heart rate
 */
void heart_rate_srv_update(const heart_rate_srv_t *srv, const uint8_t *value);

#endif /*IOTPROJECT_HEART_RATE_SERVICE_H */