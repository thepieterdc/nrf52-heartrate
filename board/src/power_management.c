/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include "power_management.h"

#include "app_error.h"
#include "nrf_pwr_mgmt.h"

void power_management_init() {
    const ret_code_t result = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(result);
}

void power_management_run() {
    nrf_pwr_mgmt_run();
}