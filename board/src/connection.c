/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "connection.h"

#include "app_error.h"
#include "app_timer.h"
#include "ble_conn_params.h"
#include "nrf_ble_qwr.h"
#include "nrf_sdh_ble.h"

/**
 * Connection handle.
 */
static uint16_t handle = BLE_CONN_HANDLE_INVALID;

/**
 * Queued writing module.
 */
NRF_BLE_QWR_DEF(queue);

void connection_parameters_init() {
    // Configure the connection parameters.
    const ble_conn_params_init_t params = {
            .p_conn_params = NULL,
            .first_conn_params_update_delay = APP_TIMER_TICKS(20000),
            .next_conn_params_update_delay = APP_TIMER_TICKS(5000),
            .max_conn_params_update_count = 3,
            .start_on_notify_cccd_handle = BLE_GATT_HANDLE_INVALID,
            .disconnect_on_fail = true,
            .evt_handler = NULL,
            .error_handler = NULL
    };

    ret_code_t result = ble_conn_params_init(&params);
    APP_ERROR_CHECK(result);

    // Configure the queued write module.
    const nrf_ble_qwr_init_t qwr_init = {
            .error_handler = app_error_handler_bare
    };
    result = nrf_ble_qwr_init(&queue, &qwr_init);
    APP_ERROR_CHECK(result);
}

void connection_start(const ble_evt_t *event) {
    // Save the connection handle.
    handle = event->evt.gap_evt.conn_handle;

    const ret_code_t result = nrf_ble_qwr_conn_handle_assign(&queue, handle);
    APP_ERROR_CHECK(result);
}

void connection_disconnect() {
    handle = BLE_CONN_HANDLE_INVALID;
}