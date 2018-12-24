#pragma once

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <regex>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include "json/json.h"
#include "JSON.h"
#include "Tools.h"
#include "BaiduPcs.h"

#include "MRApp.h"
#define BUF_SIZE 256

using std::string;
using std::thread;




class BaiduPcsSync : public MRApp {
public:
    BaiduPcsSync();
    virtual ~BaiduPcsSync(){};

    virtual void onLaunched(const std::vector<std::string> &parameters);
    virtual std::string onParameterRecieved(const std::string &params);
};
