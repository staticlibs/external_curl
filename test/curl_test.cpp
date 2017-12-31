/*
 * Copyright 2015, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   curl_test.cpp
 * Author: alex
 *
 * Created on February 10, 2015, 11:25 AM
 */


#include "curl/curl.h"

#include <iostream>

#include "staticlib/config/assert.hpp"


int main() {
    CURL* curl = curl_easy_init();
    slassert(curl);
    curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/staticlibs");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    auto res = curl_easy_perform(curl);
    if (CURLE_OK != res) {
        puts(curl_easy_strerror(res));
    } 
    curl_easy_cleanup(curl);
    slassert(CURLE_OK == res);

    return 0;
}
