/**
 * Copyright (c) 2019. All rights reserved.
 *
 * @author Freija Verbeke
 * @author Mathieu Coussens
 * @author Pieter De Clercq
 */

#include <curl/curl.h>
#include <chrono>
#include <iostream>
#include "client.h"
#include "../util/logging.h"

#define MAX_QUEUE_SIZE 10

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

web::client::client(std::string device, std::string ping_url,
                    std::string params) : device(std::move(device)),
                                          parameters_url(std::move(params)),
                                          ping_url(std::move(ping_url)) {
    // Initialise libcurl globally.
    curl_global_init(CURL_GLOBAL_ALL);
}

web::client::~client() {
    if (!this->queue.empty()) {
        this->flush();
    }
}

void web::client::flush() {
    CURL *curl;
    CURLcode res;

    json postdata;
    postdata["device"] = this->device;
    postdata["measurements"] = json::array();
    for (auto const &item : this->queue) {
        json measurement;
        measurement["timestamp"] = item.first;
        measurement["value"] = item.second;
        postdata["measurements"].push_back(measurement);
    }

    // Count the amount of items.
    const size_t items = this->queue.size();

    // Clear the queue.
    this->queue.clear();

    curl = curl_easy_init();
    if (curl != nullptr) {
        res = curl_easy_setopt(curl, CURLOPT_URL, this->ping_url.c_str());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set url.");
            exit(EXIT_FAILURE);
        }

        std::string postdata_str = postdata.dump();

        res = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE,
                               postdata_str.length());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set post data length.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS,
                               postdata_str.c_str());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set post data.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefn);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set write function.");
            exit(EXIT_FAILURE);
        }

        std::string readBuffer;
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set write container.");
            exit(EXIT_FAILURE);
        }

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");

        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (res != CURLE_OK) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set HTTP headers.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            util::logging::error(
                    "CURL: Could not execute cURL request. Host might be down."
            );
            exit(EXIT_FAILURE);
        }

        util::logging::success(
                "Published " + std::to_string(items) + " values."
        );
    } else {
        util::logging::error("Failed initialising cURL.");
        exit(EXIT_FAILURE);
    }
}

json web::client::parameters() const {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl != nullptr) {
        res = curl_easy_setopt(curl, CURLOPT_URL, this->parameters_url.c_str());
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set url.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writefn);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set write function.");
            exit(EXIT_FAILURE);
        }

        std::string readBuffer;
        res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set write container.");
            exit(EXIT_FAILURE);
        }

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Accept: application/json");

        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (res != CURLE_OK) {
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            util::logging::error("CURL: Could not set HTTP headers.");
            exit(EXIT_FAILURE);
        }

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            util::logging::error(
                    "CURL: Could not execute cURL request. Host might be down."
            );
            exit(EXIT_FAILURE);
        }

        return json::parse(readBuffer);
    }

    util::logging::error("Failed initialising cURL.");
    exit(EXIT_FAILURE);
}

void web::client::publish(const float value) {
    // Get the current timestamp.
    const auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    this->queue.insert(std::pair<int64_t, float>(timestamp, value));
    if (this->queue.size() >= MAX_QUEUE_SIZE) {
        this->flush();
    }
}