/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <iostream>
#include <ctime>
#include <chrono>
#include "logging.h"

#define COLOUR_OUT_BRIGHT_RED    "\x1B[1;31m"
#define COLOUR_OUT_CYAN          "\x1B[36m"
#define COLOUR_OUT_GREEN         "\x1B[32m"
#define COLOUR_OUT_RESET         "\x1B[0m"

/**
 * Writes a log message to stdout or stderr.
 *
 * @param type the type of the message
 * @param msg the error message
 */
static void output_log(char type, const std::string &msg) {
    std::string color;
    bool err = false;
    std::string typemsg;

    switch (type) {
        case 'E':
            color = COLOUR_OUT_BRIGHT_RED;
            err = true;
            typemsg = "ERROR";
            break;

        case 'I':
            color = COLOUR_OUT_CYAN;
            typemsg = "INFO";
            break;

        case 'S':
            color = COLOUR_OUT_GREEN;
            err = true;
            typemsg = "OK";
            break;

        default:
            color = "";
            typemsg = "DEBUG";
    }

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100] = {0};
    std::strftime(timestr, sizeof(timestr), "%H:%M:%S", std::localtime(&now));

    if (err) {
        std::cerr << '[' << timestr << "] "
                  << color << typemsg << COLOUR_OUT_RESET << ' '
                  << msg << std::endl;
    } else {
        std::cout << '[' << timestr << "] "
                  << color << typemsg << COLOUR_OUT_RESET << ' '
                  << msg << std::endl;
    }
}

void util::logging::error(const std::string &msg) {
    output_log('E', msg);
}

void util::logging::info(const std::string &msg) {
    output_log('I', msg);
}

void util::logging::success(const std::string &msg) {
    output_log('S', msg);
}