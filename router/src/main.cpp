#include <iostream>
#include <unistd.h>
#include "util/logging_util.h"
#include "web/client.h"
#include "bt/IOTBTClient.h"

/**
 * Entrypoint.
 *
 * @param argc amount of arguments
 * @param argv argument vector
 * @return 0 on success
 */
int main(int argc, char **argv) {
    if (argc != 3) {
        throw_error("Syntax: ./router https://web.service.url/ deviceId");
        exit(EXIT_FAILURE);
    }

    const std::string device(argv[2]);
    const std::string url(argv[1]);

    log_info("Device: " + device);
    log_info("Service: " + url);

    // Create a client.
    Client client(device, url);

    IOTBTClient btClient;

    // Example loop.
    // TODO: replace by bluetooth-y code.
    srand(time(nullptr));
    while(true) {
        // Obtain value from the device.
//        // TODO: obtain this value from the device.
//        const double value = rand() % 50;
//        log_info("Publishing value: " + std::to_string(value));
//
//        // Publish the value to the webservice and obtain the parameter values.
//        const auto parameters = client.publish(value);
//
//        // TODO: send this value to the device.
//        const auto frequency = parameters["frequency"].get<double>();
//        log_info("Parameter \"frequency\" value: " + std::to_string(frequency));
        char buffer[2048];
        btClient.readMessage(buffer, 128);
        printf("%s\n", buffer);
        sleep(10);
    }

    return EXIT_SUCCESS;
}
