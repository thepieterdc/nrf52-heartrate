/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef ROUTER_UTIL_LOGGING_H
#define ROUTER_UTIL_LOGGING_H

#include <string>

namespace util::logging {
    /**
     * Throws an error message.
     *
     * @param msg the message contents
     */
    void error(const std::string &msg);

    /**
     * Logs an informational message.
     *
     * @param msg the message contents
     */
    void info(const std::string &msg);

    /**
     * Logs a success message.
     *
     * @param msg the message to log
     */
    void success(const std::string &msg);
}

#endif /* ROUTER_UTIL_LOGGING_H */