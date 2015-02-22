/* 
 * File:   curl_test.cpp
 * Author: alex
 *
 * Created on February 10, 2015, 11:25 AM
 */


#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>

#include "curl/curl.h"


int main() {
    CURL* curl = curl_easy_init();
    assert(curl);
    curl_easy_setopt(curl, CURLOPT_URL, "https://google.com");
    auto res = curl_easy_perform(curl);
    assert(CURLE_OK == res);

    return 0;
}
