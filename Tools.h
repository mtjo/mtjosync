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
#define MIN_PIECES 2

class Tools {

public:
    static string runCommand(string command);

    static int saveData(string key, string value);

    static string getData(string key);

    static string getParams(const string params);

    static string getParamsByKey(const string params, string key);

    static string getUrl(const string url);

    static string postUrl(const string url, const string postData);

    static int download(string downloadUrl, string savePath);

    static int upload(string uploadUrl, string localFilePath);

    static string urlEncode(const string& str);

    static string urlDecode(const string& str);

    static void log(string logStr);

    static void fileSplit(string filePath, int pieces);

    static void fileMerge(string divName, string fileOutputPath);

    static bool fileExists(string filePath);

    static bool pathExists(string path);

};
