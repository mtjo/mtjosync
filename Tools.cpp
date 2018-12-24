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

size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream) {
    //cout << "----->reply" << endl;
    string *str = (string *) stream;
    cout << *str << endl;
    (*str).append((char *) ptr, size * nmemb);
    return size * nmemb;
}

std::string
Tools::getUrl(const std::string url) {
    std::string response;
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    //CURLcode res;
    if (curl) {
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
        //res = curl_easy_perform(curl);
        curl_easy_perform(curl);
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
    //CURLcode res;
    if (curl) {
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
        //res = curl_easy_perform(curl);
        curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return response;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int
Tools::download(std::string downloadUrl, std::string savePath) {

    CURL *curl;
    FILE *fp;
    //CURLcode res;
    const char *url = downloadUrl.data();
    char outfilename[FILENAME_MAX];

    savePath.copy(outfilename, FILENAME_MAX, 0);
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        //res = curl_easy_perform(curl);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 0;
}

//size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
//    FILE *fptr = (FILE*)userp;
//    fwrite(buffer, size, nmemb, fptr);
//}

int
Tools::upload(std::string uploadUrl, char *filename) {
    CURL *curl;
    //CURLcode res;
    FILE *fptr;
    //struct curl_slist *http_header = NULL;

    if ((fptr = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "fopen file error: %s\n", filename);
        return 1;
    }

    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fptr);

    struct curl_httppost *formpost = 0;
    struct curl_httppost *lastptr = 0;
    curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "reqformat", CURLFORM_PTRCONTENTS, "plain", CURLFORM_END);
    curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "file", CURLFORM_FILE, "/Users/hanyanyan/xx.gif", CURLFORM_END);
    curl_easy_setopt(curl, CURLOPT_URL, uploadUrl.data());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    //res = curl_easy_perform(curl);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return 0;
}


unsigned char ToHex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x) {
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string Tools::urlEncode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char) str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += ToHex((unsigned char) str[i] >> 4);
            strTemp += ToHex((unsigned char) str[i] % 16);
        }
    }
    return strTemp;
}

std::string Tools::urlDecode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%') {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char) str[++i]);
            unsigned char low = FromHex((unsigned char) str[++i]);
            strTemp += high * 16 + low;
        } else strTemp += str[i];
    }
    return strTemp;
}




