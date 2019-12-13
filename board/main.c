/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"

#include "advertising.h"
#include "connection.h"
#include "heart_rate_service.h"
#include "leds.h"
#include "power_management.h"
#include "rng.h"
#include "timers.h"

/**
 * Whether the leds should flicker or not. The leds will not flicker when the
 * a device is connected.
 */
static bool connected = false;
/**
 * Whether the leds are currently on.
 */
static bool leds_status = false;

/**
 * Instance of the heart rate service.
 */
static heart_rate_srv_t hrsrv_instance;

/**
 * Handles Bluetooth LE events.
 *
 * @param event the event to handle
 * @param unused
 */
static void event_handler(const ble_evt_t *event, void *unused) {
    switch (event->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            // Handle the connection.
            connection_start(event);
            connected = true;
            leds_set_all(true);
            break;

        case BLE_GATTC_EVT_TIMEOUT:
        case BLE_GATTS_EVT_TIMEOUT:
        case BLE_GAP_EVT_DISCONNECTED:
            // Handle the disconnected event.
            connection_disconnect();
            connected = false;
            leds_set_all(false);

            // Start advertising again to allow subsequent connections.
            advertising_start();
            break;

        default:
            // Ignore.
            break;
    }
}

/**
 * Initialises the BLE stack.
 */
static void ble_initialise() {
    ret_code_t result = nrf_sdh_enable_request();
    APP_ERROR_CHECK(result);

    // Use the default configuration.
    uint32_t ram_start = 0;
    result = nrf_sdh_ble_default_cfg_set(1, &ram_start);
    APP_ERROR_CHECK(result);

    // Enable the BLE stack.
    result = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(result);

    // Register the event handlers.
    NRF_SDH_BLE_OBSERVER(ble_observer, 3, event_handler, NULL);
    NRF_SDH_BLE_OBSERVER(hrs_observer, 3, heart_rate_srv_event,
                         (void *) &hrsrv_instance);
}

/**
 * Called by the heart rate timer.
 *
 * @param unused
 */
static void on_heart_rate_timer(void *unused) {
    UNUSED_PARAMETER(unused);
    const uint8_t heart_rate = heart_rate_srv_random(&hrsrv_instance);
    heart_rate_srv_update(&hrsrv_instance, &heart_rate);
}

/**
 * Called by the leds timer.
 *
 * @param unused
 */
static void on_leds_timer(void *unused) {
    UNUSED_PARAMETER(unused);
    if (!connected) {
        leds_set_all(leds_status);
        leds_status = !leds_status;
    }
}

/**
 * Main entrypoint.
 *
 * @return 0 on success
 */
int main() {
    // Initialise the leds.
    leds_init();

    // Initialise the timers.
    timers_init(&on_heart_rate_timer, &on_leds_timer);

    // Initialise the power management.
    power_management_init();

    // Initialise the Bluetooth LE stack.
    ble_initialise();

    // Initialise the heart rate service.
    heart_rate_srv_init(&hrsrv_instance);

    // Initialise the advertising module.
    advertising_init();

    // Initialise the connection parameters.
    connection_parameters_init();

    // Start advertising.
    advertising_start();

    // Start the timers.
    timer_start_heart_rate();
    timer_start_leds();

    // Main loop.
    while (true) {
        // Power management.
        power_management_run();
    }

    return EXIT_SUCCESS;
}