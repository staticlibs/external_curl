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
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    auto res = curl_easy_perform(curl);
    if (CURLE_OK != res) {
        puts(curl_easy_strerror(res));
    } 
    curl_easy_cleanup(curl);
    assert(CURLE_OK == res);

    return 0;
}
