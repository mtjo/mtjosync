#include "Tools.h"
#include <string>
#include "PluginTools.h"
#include "json/json.h"
#include "JSON.h"
#include <iostream>

using router::PluginTools;
using std::string;
using namespace std;

std::string
Tools::runCommand(std::string command) {
    std::string output = "";
    router::PluginTools::sCallSystem(command, output);
    return output;
}

int
Tools::saveData(const std::string key, const std::string value) {
    return router::PluginTools::saveData(key, value);
}

std::string
Tools::getData(std::string key) {
    std::string value = "";
    router::PluginTools::getData(key, value);
    return value;
}

std::string
Tools::getParams(const std::string params) {
    const char *ch = params.data();
    struct json_object *jsonObject = NULL;
    jsonObject = json_tokener_parse(ch);
    std::string data = "";
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        jsonObject = json_object_object_get(jsonObject, "data");
        data = json_object_get_string(jsonObject);
    }
    json_object_put(jsonObject);
    return data;
}

std::string
Tools::getParamsByKey(const std::string params, std::string key) {
    const char *ch = params.data();
    struct json_object *jsonObject = NULL;
    jsonObject = json_tokener_parse(ch);
    std::string data = "";
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        jsonObject = json_object_object_get(jsonObject, key.data());
        data = json_object_get_string(jsonObject);
    }
    json_object_put(jsonObject);
    return data;
}

size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
    //cout << "----->reply" << endl;
    string *str = (string*)stream;
    cout << *str << endl;
    (*str).append((char*)ptr, size*nmemb);
    return size * nmemb;
}

std::string
Tools::getUrl(const std::string url) {
    std::string response;
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return response;
}

std::string
Tools::postUrl(const std::string url, const std::string postParams) {
    std::string response = 0;

    // init curl
    CURL *curl = curl_easy_init();
    // res code
    CURLcode res;
    if (curl)
    {
        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        res = curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return response;
}


void Tools::urldecode(char *dst, const char *src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a'-'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a'-'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16*a+b;
            src+=3;
        } else if (*src == '+') {
            *dst++ = ' ';
            src++;
        } else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}

