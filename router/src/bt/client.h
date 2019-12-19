/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef ROUTER_BT_CLIENT_H
#define ROUTER_BT_CLIENT_H

#include <gattlib.h>
#include <string>
#include <unistd.h>
#include "../web/client.h"

/**
 * Callbacks to heart rate notifications.
 *
 * @param the id of the characteristic
 * @param the data
 * @param the length of the data
 * @param the client instance
 */
typedef void bt_client_notification_t(const uuid_t *, const uint8_t *, size_t,
                                      void *);

namespace bt {
    /**
     * A client that communicates with the device using BLE.
     */
    class client {
    private:
        const std::string &address;
        gatt_connection_t *connection;
    public:
        /**
         * client constructor.
         *
         * @param address the address of the device to communicate with
         */
        explicit client(const std::string &address);

        /**
         * client destructor.
         */
        virtual ~client();

        /**
         * Attempt to connect to the device.
         *
         * @return true if the connection was successful
         */
        bool connect();

        /**
         * Listens for notifications of the heart rate measurement.
         *
         * @param callback the callback to execute when receiving a new
         * notification
         * @param ctx context to pass to the callback
         */
        void listen(bt_client_notification_t callback, void *ctx);

        /**
         * Updates the parameter values on the device.
         *
         * @param max the max threshold
         * @param min the min threshold
         */
        void update_parameters(std::uint8_t max, std::uint8_t min) const;
    };
}

#endif /* ROUTER_BT_CLIENT_H */
