#pragma once
#include "json/json.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string>
#include "PluginTools.h"
#include "json/json.h"
#include "JSON.h"
#include <string.h>
#include <iostream>
#include <sstream>

using router::PluginTools;
using std::string;
using std::stringstream;
using namespace std;


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

    static int upload(std::string uploadUrl, std::string localFilePath);

    static std::string urlEncode(const std::string& str);

    static std::string urlDecode(const std::string& str);

    static void log(std::string logStr);

    static void fileSplit(std::string filePath);

    static void fileMerge(std::string divName, std::string fileOutputPath);

    static bool fileExists(std::string filePath);

    static bool pathExists(std::string path);

};
