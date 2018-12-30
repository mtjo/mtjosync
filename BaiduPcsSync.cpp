#include "BaiduPcsSync.h"


struct cb_arg
{
    struct event *ev;
    struct timeval tv;
};

void timeout_cb(int fd, short event, void *params)
{

    struct cb_arg *arg = (struct cb_arg*)params;
    struct event *ev = arg->ev;
    struct timeval tv = arg->tv;

    evtimer_add(ev, &tv);
}




BaiduPcsSync::BaiduPcsSync() {
}


void startSync() {

    struct event_base *base = event_base_new();
    struct event *timeout = NULL;
    struct timeval tv = {1, 0};
    struct cb_arg arg;

    timeout = evtimer_new(base, timeout_cb, &arg);
    arg.ev = timeout;
    arg.tv = tv;
    evtimer_add(timeout, &tv);
    event_base_dispatch(base);
    evtimer_del(timeout);

    //Tools::runCommand("pcsSync >> /sync.log");
}

void runSync() {
//    std::string res = BaiduPcs::getPcsFileMeta("/sync/conf.d.zip");
//    std::string res1 = BaiduPcs::downloadPcsFile("/sync/conf.d.zip", "/conf.d.zip");
//    std::string res2 = BaiduPcs::getPcsFileList("/sync", 0, 10);
//    std::string res3 = BaiduPcs::uploadFile2Pcs("/userdisk/new.jpg", "/new.jpg", "overwrite");

//    Tools::fileSplit("/conf.d.zip", 6);
//
//    Tools::fileSplit("/userdata/共享/Pacifist355.dmg", 10);
//
//    Tools::fileSplit("/userdata/软件/Untitled-1.dmg", 10);
//
//    Tools::fileSplit("/userdata/软件/Microsoft_Office_2016_15.41.17120500_Installer.pkg", 10);
//
//    Tools::fileMerge("/.conf.d.zip", "/conf.d_new.zip");
//    Tools::fileMerge("/userdata/共享/.Pacifist355.dmg", "/userdata/共享/Pacifist355-new.dmg");

    BaiduPcs::burstDownloadPcsFile("/sync/Y450 Driver V3.0.iso", "/testconf.d.zip", 5);

}

//
void killAutorun() {
    Tools::runCommand("killall pcsSync");
}


void
BaiduPcsSync::onLaunched(const std::vector <std::string> &parameters) {
    std::thread subthread(startSync);
    subthread.detach();

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

        std::string res = BaiduPcs::getPcsQuota();
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

            std::string hasSet = Tools::runCommand("grep " + _key + " " + config_file);
            if (hasSet == "") {
                Tools::runCommand("echo " + _key + "=" + _value + " >> " + config_file);
            }
            Tools::runCommand("sed -i 's/" + _key + "=.*/" + _key + "=" + _value + "/g' " + config_file);

            data.put(_key, "pcsSync config  " + _key + " " + _value);
        }

        return JSONObject::success(data);
    } else if (method == "startSync") {
        std::thread subthread(startSync);
        subthread.detach();
        return JSONObject::success("startSync");
    } else if (method == "runSync") {
        std::thread subthread(runSync);
        subthread.detach();
        return JSONObject::success("runSync");
    }

    return JSONObject::error(1, "parameter missing");


}

BaiduPcsSync baiduPcsSync;
