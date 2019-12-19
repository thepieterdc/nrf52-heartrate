/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <unistd.h>
#include <iostream>
#include <thread>
#include "bt/client.h"
#include "util/logging.h"
#include "web/client.h"

void on_notification(const uuid_t *uuid, const uint8_t *data,
                     size_t data_length, void *web) {
    // Convert the value to a float.
    const float heart_rate = data[0];

    // Publish the heart rate.
    auto *client = static_cast<web::client *>(web);
    client->publish(heart_rate);
}

/**
 * Entrypoint.
 *
 * @param argc amount of arguments
 * @param argv argument vector
 * @return 0 on success
 */
int main(int argc, char **argv) {
    if (argc != 3) {
        util::logging::error(
                "Syntax: ./router https://web.service.url/ device_address");
        exit(EXIT_FAILURE);
    }

    const std::string device_address(argv[2]);
    const std::string url(argv[1]);

    if (device_address.length() != 17) {
        util::logging::error("Address of the device should be 17 characters.");
        exit(EXIT_FAILURE);
    }

    util::logging::info("Device: " + device_address);
    util::logging::info("Service: " + url);

    // Create a web client.
    web::client web_client(device_address, url + "/ping",
                           url + "/parameters?device=" + device_address);

    // Create a bluetooth client.
    bt::client bt_client(device_address);

    // Attempt to connect to the device.
    util::logging::info("Connecting to the device...");
    if (bt_client.connect()) {
        util::logging::success("Connected to the device.");
    } else {
        util::logging::error("Could not connect to the device.");
        exit(EXIT_FAILURE);
    }

    // Listen for notifications.
    bt_client.listen(on_notification, &web_client);

    // Main loop: Update parameters on the device.
    std::uint8_t prev_max = 0;
    std::uint8_t prev_min = 0;
    while (true) {
        // Obtain the parameter values.
        const json parameters = web_client.parameters();

        // Convert the parameters to the correct format.
        const auto max = parameters["max"].get<std::uint8_t>();
        const auto min = parameters["min"].get<std::uint8_t>();

        if (max == prev_max && min == prev_min) {
            // Same values, ignore.
            continue;
        }

        bt_client.update_parameters(max, min);

        prev_max = max;
        prev_min = min;

        // Wait 5 seconds.
        sleep(5);
    }

    return EXIT_SUCCESS;
}
