#include "BaiduPcsSync.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
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
//#include "boost/thread.hpp"

using std::string;
using std::thread;

#define BUF_SIZE 256

BaiduPcsSync::BaiduPcsSync() {
}
//void startFrpc(){
//    system("/frp/frpc_autorun.sh");
//}
//
//void killAutorun(){
//    system("sleep 2 && killall \"frpc_autorun.sh\">>./shell.log");
//}


void
BaiduPcsSync::onLaunched(const std::vector <std::string> &parameters) {
//    std::thread subthread(startFrpc);
//    subthread.detach();
//
//    std::thread killthread(killAutorun);
//    killthread.detach();

};


std::string
BaiduPcsSync::onParameterRecieved(const std::string &params) {
    std::string method = Tools::getParamsByKey(params, "method");
    JSONObject data;
    if (method == "") {
        return JSONObject::error(999, "method can not be null");
    } else if (method == "getQuota") {
        std::string pcsConfig = Tools::getData("pcsConfig");

        const char *input = pcsConfig.data();
        char *output = (char *) malloc(strlen(input) + 1);

        Tools::urldecode(output, input);
        //data.put("pcsConfig",pcsConfig);
        pcsConfig = output;
        //data.put("output",pcsConfig);


        std::string accessToken = Tools::getParamsByKey(pcsConfig, "access_token");
        const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" + accessToken;

        std::string res = Tools::getUrl(strUrl);
        std::string quota = Tools::getParamsByKey(res, "quota");
        std::string used = Tools::getParamsByKey(res, "used");

        data.put("quota", quota);
        data.put("used", used);

        return JSONObject::success(data);
    } else if (method == "savePcs") {
        std::string configStr = Tools::getParamsByKey(params, "configData");
        const char *ch = configStr.data();
        struct json_object *configData = json_tokener_parse(ch);

        json_object_object_foreach(configData, key, value)
        {
            std::string _key = key;
            std::string _value = json_object_get_string(value);
            std::string config_file = "/bin/syncy.conf";
            std::string command = "grep " + _key + " " + config_file + ">/dev/null 2>&1 || { echo " + _key + "=" + _value + " >> "+config_file+"; }";

            Tools::runCommand(command);
            Tools::runCommand("sed -i 's/" + _key + "=.*/" + _key + "=" + _value + "/g' " + config_file);

            data.put(_key, "pcsSync config  " + _key + " " + _value);
        }

        return JSONObject::success(data);
    }

    return JSONObject::error(1, "parameter missing");


}

BaiduPcsSync baiduPcsSync;
