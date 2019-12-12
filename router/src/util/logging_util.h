/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef ROUTER_UTIL_LOGGING_UTIL_H
#define ROUTER_UTIL_LOGGING_UTIL_H

#include <string>

/**
 * Logs an informational message.
 *
 * @param msg the message contents
 */
void log_info(const std::string &msg);

/**
 * Logs a success message.
 *
 * @param msg the message to log
 */
void log_success(const std::string &msg);

/**
 * Throws an error message.
 *
 * @param msg the message contents
 */
void throw_error(const std::string &msg);

#endif /* ROUTER_UTIL_LOGGING_UTIL_H */