/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef ROUTER_WEB_CLIENT_H
#define ROUTER_WEB_CLIENT_H

#include <json.hpp>
#include <string>
#include <utility>

using json = nlohmann::json;

namespace web {
    /**
     * Web client.
     */
    class client {
    private:
        std::string device;
        std::map<std::int64_t, float> queue;
        std::string parameters_url;
        std::string ping_url;

        /**
         * Flushes the queue and pushes all values to the server.
         */
        void flush();

    public:
        /**
         * client constructor.
         *
         * @param device the device id
         * @param url the web service ping url
         * @param url the web service parameters url
         */
        client(std::string device, std::string ping_url, std::string params);

        /**
         * client destructor.
         */
        ~client();

        /**
         * Gets the parameter values as json.
         *
         * @return the json response
         */
        json parameters() const;

        /**
         * Publishes the given value to the web service.
         *
         * @param value the value to publish
         * @return the response
         */
        void publish(float value);
    };
}

#endif /* ROUTER_WEB_CLIENT_H */