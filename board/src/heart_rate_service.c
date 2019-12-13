/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "heart_rate_service.h"

#include <string.h>
#include <rng.h>
#include "leds.h"
#include "app_error.h"
#include "nrf_sdh_ble.h"

/**
 * The length of the values.
 */
static uint16_t bounds_length = 2;
static uint16_t measurement_length = 1;

#define HEART_RATE_INITIAL_MAX 80
#define HEART_RATE_INITIAL 60
#define HEART_RATE_INITIAL_MIN 45

/**
 * Adds the heart rate bounds service.
 *
 * @param srv service instance
 */
static void add_heart_rate_bounds_service(heart_rate_srv_t *srv) {
    // Create the service uuid.
    ble_uuid_t srv_uuid;
    ble_uuid128_t base_uid = IOTPROJECT_UUID;
    srv_uuid.uuid = HEART_RATE_BOUNDS_SERVICE;
    ret_code_t result = sd_ble_uuid_vs_add(&base_uid, &srv_uuid.type);
    APP_ERROR_CHECK(result);

    // Register the service.
    result = sd_ble_gatts_service_add(
            BLE_GATTS_SRVC_TYPE_PRIMARY,
            &srv_uuid,
            &srv->heart_rate_bounds_srv_handle);
    APP_ERROR_CHECK(result);

    // Characteristic UUID configuration.
    ble_uuid_t char_uuid;
    char_uuid.uuid = HEART_RATE_BOUNDS_CHARACTERISTIC;
    result = sd_ble_uuid_vs_add(&base_uid, &char_uuid.type);
    APP_ERROR_CHECK(result);

    // Set the metadata.
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    // Set the client characteristic configuration descriptor metadata.
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.char_props.notify = 0;
    char_md.p_cccd_md = &cccd_md;

    // Set the attribute metadata.
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    // Set security levels of the characteristic.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    // Set the characteristic value attribute.
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.init_len = bounds_length;
    attr_char_value.max_len = bounds_length;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.p_uuid = &char_uuid;

    // Set the initial value.
    uint8_t value[2] = {HEART_RATE_INITIAL_MAX, HEART_RATE_INITIAL_MIN};
    attr_char_value.p_value = value;

    result = sd_ble_gatts_characteristic_add(
            srv->heart_rate_bounds_srv_handle,
            &char_md, &attr_char_value,
            &srv->heart_rate_bound_handles);
    APP_ERROR_CHECK(result);
}

/**
 * Adds the heart rate measurement service.
 *
 * @param srv service instance
 */
static void add_heart_rate_measurement_service(heart_rate_srv_t *srv) {
    // Create the service uuid.
    ble_uuid_t srv_uuid;
    ble_uuid128_t base_uid = IOTPROJECT_UUID;
    srv_uuid.uuid = HEART_RATE_MEASUREMENT_SERVICE;
    ret_code_t result = sd_ble_uuid_vs_add(&base_uid, &srv_uuid.type);
    APP_ERROR_CHECK(result);

    // Register the service.
    result = sd_ble_gatts_service_add(
            BLE_GATTS_SRVC_TYPE_PRIMARY,
            &srv_uuid,
            &srv->heart_rate_measurement_srv_handle);
    APP_ERROR_CHECK(result);

    // Characteristic UUID configuration.
    ble_uuid_t char_uuid;
    char_uuid.uuid = HEART_RATE_MEASUREMENT_CHARACTERISTIC;
    result = sd_ble_uuid_vs_add(&base_uid, &char_uuid.type);
    APP_ERROR_CHECK(result);

    // Set the metadata.
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 0;

    // Set the client characteristic configuration descriptor metadata.
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    char_md.char_props.notify = 1;
    char_md.p_cccd_md = &cccd_md;

    // Set the attribute metadata.
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));
    attr_md.vloc = BLE_GATTS_VLOC_STACK;

    // Set security levels of the characteristic.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    // Set the characteristic value attribute.
    ble_gatts_attr_t attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.init_len = measurement_length;
    attr_char_value.max_len = measurement_length;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.p_uuid = &char_uuid;

    // Set the initial value.
    uint8_t value[1] = {HEART_RATE_INITIAL};
    attr_char_value.p_value = value;

    result = sd_ble_gatts_characteristic_add(
            srv->heart_rate_measurement_srv_handle,
            &char_md, &attr_char_value,
            &srv->heart_rate_measurement_handles);
    APP_ERROR_CHECK(result);
}

/**
 * Configures the bounds of the heart rate simulator.
 *
 * @param srv the service
 * @param event the event
 */
static void set_bounds(heart_rate_srv_t *srv, const ble_evt_t *event) {
    // Extract the write event.
    const ble_gatts_evt_write_t *write = &event->evt.gatts_evt.params.write;

    // Check the length of the event.
    if (write->len == 2) {
        const uint8_t maxval = write->data[0];
        const uint8_t minval = write->data[1];

        // Maximum must be greater than minimum.
        if (minval >= maxval) {
            return;
        }

        // Maximum must be within bounds.
        if (maxval > HEART_RATE_MAX || maxval < HEART_RATE_MIN) {
            return;
        }

        // Minimum must be within bounds.
        if (minval > HEART_RATE_MAX || minval < HEART_RATE_MIN) {
            return;
        }

        // Change the bounds.
        srv->min = minval;
        srv->max = maxval;
    }
}

void heart_rate_srv_event(const ble_evt_t *event, void *hrs_instance) {
    heart_rate_srv_t *srv = (heart_rate_srv_t *) hrs_instance;
    switch (event->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            srv->conn_handle = event->evt.gap_evt.conn_handle;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            srv->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;

        case BLE_GATTS_EVT_WRITE:
            set_bounds(srv, event);
            break;

        default:
            break;
    }
}

void heart_rate_srv_init(heart_rate_srv_t *instance) {
    // Construct the service.
    memset(instance, 0, sizeof(&instance));
    instance->conn_handle = BLE_CONN_HANDLE_INVALID;
    instance->max = HEART_RATE_INITIAL_MAX;
    instance->min = HEART_RATE_INITIAL_MIN;
    instance->previous = HEART_RATE_INITIAL;

    // Register the heart rate measurement service.
    add_heart_rate_measurement_service(instance);

    // Register the heart rate bounds service.
    add_heart_rate_bounds_service(instance);
}

uint8_t heart_rate_srv_random(heart_rate_srv_t *instance) {
    // Generate a random value between 0 and 255.
    const uint8_t random = rng_generate_uint8();

    // Cast it to a value between 0 and 8.
    const uint8_t delta = (uint8_t) ((((float) random) / 255.f) * 8.f);

    uint8_t nw = instance->previous;
    if (delta < 4) {
        nw -= delta;
    } else {
        nw += (delta - 4);
    }

    // Ensure within bounds.
    if (nw >= instance->max) {
        nw = instance->max;
    } else if (nw <= instance->min) {
        nw = instance->min;
    }

    // Return the new value.
    instance->previous = nw;
    return nw;
}

void heart_rate_srv_update(const heart_rate_srv_t *srv, const uint8_t *value) {
    if (srv->conn_handle != BLE_CONN_HANDLE_INVALID) {
        // Set the notification parameters.
        ble_gatts_hvx_params_t hvx_params;
        memset(&hvx_params, 0, sizeof(hvx_params));
        hvx_params.handle = srv->heart_rate_measurement_handles.value_handle;
        hvx_params.offset = 0;
        hvx_params.p_data = value;
        hvx_params.p_len = &measurement_length;
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;

        // Publish the update.
        sd_ble_gatts_hvx(srv->conn_handle, &hvx_params);
    }
}