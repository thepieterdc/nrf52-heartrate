/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <utility>
#include "client.h"
#include "../util/logging.h"

#define HEART_BOUNDS_CHARACTERISTIC "0000aabb-0096-1227-0097-050400970821"
#define HEART_RATE_CHARACTERISTIC "0000aaab-0096-1227-0097-050400970821"

bt::client::client(const std::string &address) : address(address),
                                                 connection(nullptr) {}

bt::client::~client() {
    if (this->connection != nullptr) {
        gattlib_disconnect(this->connection);
        this->connection = nullptr;
    }
}

bool bt::client::connect() {
    this->connection = gattlib_connect(nullptr, this->address.c_str(),
                                       GATTLIB_CONNECTION_OPTIONS_LEGACY_DEFAULT);
    return this->connection != nullptr;
}

void bt::client::listen(bt_client_notification_t callback, void *ctx) {
    // Register for notifications.
    gattlib_register_notification(this->connection, callback, ctx);

    // Start listening.
    uuid_t char_uuid = {};
    char_uuid.type = SDP_UUID128;
    gattlib_string_to_uuid(HEART_RATE_CHARACTERISTIC, 37, &char_uuid);
    const auto result = gattlib_notification_start(this->connection,
                                                   &char_uuid);
    if (result != 0) {
        util::logging::error("Failed to start listening for notifications.");
        exit(EXIT_FAILURE);
    }
}

void bt::client::update_parameters(std::uint8_t max, std::uint8_t min) const {
    // Format the data.
    uint8_t data[2] = {max, min};

    // Obtain the uuid.
    uuid_t char_uuid = {};
    char_uuid.type = SDP_UUID128;
    gattlib_string_to_uuid(HEART_BOUNDS_CHARACTERISTIC, 37, &char_uuid);

    // Send the data to the device.
    const auto result = gattlib_write_char_by_uuid(this->connection, &char_uuid,
                                                   data, sizeof(data));
    if (result != 0) {
        util::logging::error("Failed to update parameters.");
        exit(EXIT_FAILURE);
    }

    util::logging::success("Updated parameters.");
}
