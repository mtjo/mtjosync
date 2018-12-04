#pragma once

#include <string>

#include "MRApp.h"

class BaiduPcsSync : public MRApp {
public:
    BaiduPcsSync();
    virtual ~BaiduPcsSync(){};

    virtual void onLaunched(const std::vector<std::string> &parameters);
    virtual std::string onParameterRecieved(const std::string &params);
};
