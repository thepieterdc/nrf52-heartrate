/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <curl/curl.h>
#include <iostream>
#include "client.h"
#include "../util/logging_util.h"

/**
 * Curl writefunction that performs a no-op.
 *
 * @param contents input buffer
 * @param size size per member
 * @param nmemb amount of members
 * @param userp buffer to store the contents in
 * @return amount of bytes written
 */
static size_t
curl_writefn(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

json Client::publish(double value) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl != nullptr) {
        res = curl_easy_setopt(curl, CURLOPT_URL, this->url.c_str());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set url.");
            exit(EXIT_FAILURE);
        }

        std::string postdata(
                "device=" + this->device + "&value=" + std::to_string(value)
        );

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postdata.length());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set post data length.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, postdata.c_str());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set post data.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefn);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set write function.");
            exit(EXIT_FAILURE);
        }

        std::string readBuffer;
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set write container.");
            exit(EXIT_FAILURE);
        }

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");

        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (res != CURLE_OK) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            throw_error("CURL: Could not set HTTP headers.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            throw_error(
                    "CURL: Could not execute cURL request. Host might be down."
            );
            exit(EXIT_FAILURE);
        }

        log_success("Published value.");

        return json::parse(readBuffer);
    }

    throw_error("Failed initialising cURL.");
    exit(EXIT_FAILURE);
}