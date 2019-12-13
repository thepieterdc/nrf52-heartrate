/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "advertising.h"

#include "app_error.h"
#include "ble_advdata.h"
#include "ble_srv_common.h"
#include "nrf_ble_gatt.h"
#include "nrf_sdh_ble.h"

#include "heart_rate_service.h"

/**
 * Pointer to the GATT instance.
 */
NRF_BLE_GATT_DEF(gatt_inst);

/**
 * Advertising data buffers.
 */
static uint8_t encoded_advert_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
static uint8_t encoded_scan_resp_data[BLE_GAP_ADV_SET_DATA_SIZE_MAX];

/**
 * Advertisement data.
 */
static ble_gap_adv_data_t advert_data = {
        .adv_data = {
                .p_data = encoded_advert_data,
                .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
        },
        .scan_rsp_data ={
                .p_data = encoded_scan_resp_data,
                .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX

        }
};

/**
 * Advertising handle.
 */
static uint8_t handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;

void advertising_init() {
    ret_code_t result;

    // Initialise the Generic Access Profile security mode.
    ble_gap_conn_sec_mode_t gap_security_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&gap_security_mode);

    // Set the advertised device name.
    result = sd_ble_gap_device_name_set(&gap_security_mode,
                                        (const uint8_t *) DEVICE_NAME,
                                        strlen(DEVICE_NAME));
    APP_ERROR_CHECK(result);

    // Set the advertised device appearance to a heart rate sensor.
    result = sd_ble_gap_appearance_set(
            BLE_APPEARANCE_GENERIC_HEART_RATE_SENSOR);
    APP_ERROR_CHECK(result);

    // Initialize the advertising data.
    ble_advdata_t raw_advert_data = {};
    memset(&raw_advert_data, 0, sizeof(raw_advert_data));
    raw_advert_data.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    raw_advert_data.include_appearance = true;
    raw_advert_data.name_type = BLE_ADVDATA_FULL_NAME;

    // Advertised uuids.
    ble_uuid_t adv_uuids[] = {
            {HEART_RATE_MEASUREMENT_SERVICE, BLE_UUID_TYPE_BLE},
            {HEART_RATE_BOUNDS_SERVICE,      BLE_UUID_TYPE_BLE}
    };

    // Initialize the scan response data.
    ble_advdata_t raw_scan_resp_data = {};
    memset(&raw_scan_resp_data, 0, sizeof(raw_scan_resp_data));
    raw_scan_resp_data.uuids_complete.p_uuids = adv_uuids;
    raw_scan_resp_data.uuids_complete.uuid_cnt = 2;

    // Encode the advertising data.
    result = ble_advdata_encode(&raw_advert_data, advert_data.adv_data.p_data,
                                &advert_data.adv_data.len);
    APP_ERROR_CHECK(result);

    result = ble_advdata_encode(&raw_scan_resp_data,
                                advert_data.scan_rsp_data.p_data,
                                &advert_data.scan_rsp_data.len);
    APP_ERROR_CHECK(result);

    // Build the advertising parameters.
    ble_gap_adv_params_t adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.duration = BLE_GAP_ADV_TIMEOUT_GENERAL_UNLIMITED;
    adv_params.filter_policy = BLE_GAP_ADV_FP_ANY;
    adv_params.interval = 800;
    adv_params.p_peer_addr = NULL;
    adv_params.primary_phy = BLE_GAP_PHY_1MBPS;
    adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;

    result = sd_ble_gap_adv_set_configure(&handle, &advert_data, &adv_params);
    APP_ERROR_CHECK(result);

    // Initialise the GAP connection parameters.
    ble_gap_conn_params_t connection_parameters = {
            .min_conn_interval = MSEC_TO_UNITS(100, UNIT_1_25_MS),
            .max_conn_interval = MSEC_TO_UNITS(200, UNIT_1_25_MS),
            .slave_latency = 0,
            .conn_sup_timeout = MSEC_TO_UNITS(4000, UNIT_10_MS)
    };
    result = sd_ble_gap_ppcp_set(&connection_parameters);
    APP_ERROR_CHECK(result);

    // Initialise the Generic Attribute Profile.
    result = nrf_ble_gatt_init(&gatt_inst, NULL);
    APP_ERROR_CHECK(result);
}

void advertising_start() {
    const ret_code_t result = sd_ble_gap_adv_start(handle, 1);
    APP_ERROR_CHECK(result);
}
