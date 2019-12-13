/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#ifndef IOTPROJECT_CONNECTION_H
#define IOTPROJECT_CONNECTION_H

#include <ble.h>

/**
 * Initialises the connection parameters.
 */
void connection_parameters_init();

/**
 * Initiates the connection.
 *
 * @param connection event
 */
void connection_start(const ble_evt_t *event);

/**
 * Abandons the connection.
 */
void connection_disconnect();

#endif /* IOTPROJECT_CONNECTION_H */
