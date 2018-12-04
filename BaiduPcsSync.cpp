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
    std::string method = Tools::getParamsByKey(params,"method");
    JSONObject data;
    if (method == "") {
        return JSONObject::error(999, "method can not be null");
    } else if (method == "getQuota") {
        std::string pcsConfig=Tools::getData("pcsConfig");

        const char *input =  pcsConfig.data();
        char *output = (char *)malloc(strlen(input)+1);

        Tools::urldecode(output, input);
        data.put("pcsConfig",pcsConfig);
        pcsConfig  = output;
        data.put("output",pcsConfig);


        std::string accessToken=Tools::getParamsByKey(pcsConfig,"access_token");
        const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token="+accessToken;

        std::string res = Tools::getUrl(strUrl);
        data.put("res",res);
        return JSONObject::success(data);
    }

    return JSONObject::error(1, "parameter missing");


}

BaiduPcsSync baiduPcsSync;
