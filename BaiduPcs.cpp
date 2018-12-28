#include "BaiduPcs.h"

string BaiduPcs::getPcsQuota() {
    string accessToken = Tools::getData("accessToken");
    const string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" + accessToken;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}

string BaiduPcs::mkdir(string pcsPath) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    string accessToken = Tools::getData("accessToken");
    const string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=mkdir&access_token=" + accessToken + "&path=" + pcsPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}

string BaiduPcs::getPcsFileMeta(string pcsPath) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    string accessToken = Tools::getData("accessToken");
    const string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=meta&access_token=" + accessToken + "&path=" + pcsPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);

}


string BaiduPcs::getPcsFileList(string pcsPath, int start, int end) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    string accessToken = Tools::getData("accessToken");
    stringstream ss;
    ss << start;
    string startStr = ss.str();
    ss << end;
    string endStr = ss.str();
    const string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/file?method=list&access_token=" + accessToken +
                          "&path=" + pcsPath +
                          "&limit=" + startStr + "-" + endStr +
                          "&by=name&order=asc";
    Tools::log(strUrl);

    return Tools::getUrl(strUrl);
}


string BaiduPcs::downloadPcsFile(string pcsFilePath, string localFilePath) {
    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    string accessToken = Tools::getData("accessToken");
    string strUrl =
            "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
            pcsFilePath;
    string command =
            "curl -sSLk -R --retry 3 -Y 1 -y 60 -A '' -o \"" + localFilePath + "\" " + "\"" + strUrl + "\"";
    Tools::log(strUrl);
    return Tools::runCommand(command);
    //Tools::download(strUrl, localFilePath);
}

//多线程分片下载
string BaiduPcs::burstDownloadPcsFile(string pcsFilePath, string localFilePath, int treadCount) {
    string pcsFileMeta = BaiduPcs::getPcsFileMeta(pcsFilePath + "22");
    Tools::log("pcsFileMeta: " + pcsFileMeta);
    struct json_object *jsonObject = json_tokener_parse(pcsFileMeta.data());
    Tools::log(json_object_get_string(jsonObject));
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        jsonObject = json_object_object_get(jsonObject, "list");
        Tools::log(json_object_get_string(jsonObject));
        //array_list *list = json_object_get_array(jsonObject);
        int length = json_object_array_length(jsonObject);
        //jsonObject = json_object_array_get_idx(list, 0);
        if (length == 1) {
            jsonObject = json_object_array_get_idx(jsonObject, 0);
            Tools::log(json_object_get_string(jsonObject));
            jsonObject = json_object_object_get(jsonObject, "size");
            int size = json_object_get_int(jsonObject);
            char maxint[100];
            sprintf(maxint, " size: %d", size);
            Tools::log(maxint);
        }

    }
    json_object_put(jsonObject);

    FILE *fsrc = fopen("/userdata/软件/Untitled-1.dmg", "rb");

    if (fsrc == NULL) {
        Tools::log("fsrc:文件打开错误");
    }

    FILE *fsrc2 = fopen("/userdata/软件/Microsoft_Office_2016_15.41.17120500_Installer.pkg", "rb");

    if (fsrc2 == NULL) {
        Tools::log("fsrc2:文件打开错误");
    }

    FILE *fsrc3 = fopen("/userdata/共享/cn_windows_8.1_with_update_x64_dvd_4048046.iso", "rb");

    if (fsrc3 == NULL) {
        Tools::log("fsrc3:文件打开错误");
    }

    Tools::fileSplit("/userdata/软件/Microsoft_Office_2016_15.41.17120500_Installer.pkg",10);


    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    string accessToken = Tools::getData("accessToken");

    string strUrl =
            "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
            pcsFilePath;
    string command =
            "curl -sSLk -R --retry 3 -Y 1 -y 60 -A '' -o \"" + localFilePath + "\" " + "\"" + strUrl + "\"";
    //Tools::log(strUrl);
    return Tools::runCommand(command);
    //Tools::download(strUrl, localFilePath);
}

string BaiduPcs::uploadFile2Pcs(string localFilePath, string pcsFilePath, string ondup) {
    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    string accessToken = Tools::getData("accessToken");
    string strUrl =
            "https://c.pcs.baidu.com/rest/2.0/pcs/file?method=upload&access_token=" + accessToken +
            "&path=" + pcsFilePath +
            "&ondup=" + ondup;
    Tools::log(strUrl);
    string command =
            "curl -sSLk --retry $retrytimes -A '' -F \"file=@\"" + localFilePath + "\"" + "\"" + strUrl + "\"";
    //Tools::upload(strUrl, localFilePath);
    return Tools::runCommand(command);

}

string BaiduPcs::movePcsFile(string pcsFromPath, string pcsToPath) {
    pcsFromPath = Tools::urlEncode(PCSROOT + pcsFromPath);
    pcsToPath = Tools::urlEncode(PCSROOT + pcsToPath);
    string accessToken = Tools::getData("accessToken");
    const string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=move&access_token=" + accessToken +
            "&from=" + pcsFromPath +
            "&to=" + pcsToPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}
