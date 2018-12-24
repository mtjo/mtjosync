#pragma once
#include <string>
#include "json/json.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

class Tools {

public:
    static std::string runCommand(std::string command);

    static int saveData(std::string key, std::string value);

    static std::string getData(std::string key);

    static std::string getParams(const std::string params);

    static std::string getParamsByKey(const std::string params, std::string key);

    static std::string getUrl(const std::string url);

    static std::string postUrl(const std::string url, const std::string postData);

    static int download(std::string downloadUrl, std::string savePath);

    static int upload(std::string uploadUrl, char *filename);

    static std::string urlEncode(const std::string& str);

    static std::string urlDecode(const std::string& str);

};
