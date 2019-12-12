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

class Client {
private:
    std::string device;
    std::string url;
public:
    /**
     * Client constructor.
     *
     * @param device the device id
     * @param url the web service url
     */
    Client(std::string device, std::string url) : device(std::move(device)),
                                                  url(std::move(url)) {};

    /**
     * Client destructor.
     */
    ~Client() = default;

    /**
     * Publishes the given value to the web service.
     *
     * @param value the value to publish
     */
    json publish(double value);
};

#endif /* ROUTER_WEB_CLIENT_H */