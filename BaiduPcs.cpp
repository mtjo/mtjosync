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



struct divFile
{
    int index;
    char *name;
    int size;
    bool len;
    long long len;
};

void threadDownalod() {

}


//多线程分片下载
string BaiduPcs::burstDownloadPcsFile(string pcsFilePath, string localFilePath, int treadCount) {


    int pieceSize = 1024 * 1024 * 500;
    string pcsFileMeta = BaiduPcs::getPcsFileMeta(pcsFilePath);
    struct json_object *jsonObject = json_tokener_parse(pcsFileMeta.data());
    Tools::log(json_object_get_string(jsonObject));
    int size = 0;
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        if (json_object_object_get_ex(jsonObject, "list", &jsonObject)) {
            Tools::log(json_object_get_string(jsonObject));
            int length = json_object_array_length(jsonObject);
            if (length == 1) {
                jsonObject = json_object_array_get_idx(jsonObject, 0);
                Tools::log(json_object_get_string(jsonObject));
                jsonObject = json_object_object_get(jsonObject, "size");
                size = json_object_get_int(jsonObject);
                char maxint[100];
                sprintf(maxint, "pcs file size: %d", size);
                Tools::log(maxint);
            }
        } else {
            Tools::log("PCS错误: " + pcsFileMeta);
        }
    }
    json_object_put(jsonObject);

    if (size == 0) {
        Tools::log("PCS文件不存在！ ");
        return "";
    }
    string filename = localFilePath.substr(localFilePath.find_last_of("/") + 1);//获取文件名
    string fileDir = localFilePath.substr(0, localFilePath.find_last_of("/") + 1);//获取文件所在路径

    Tools::runCommand("mkdir -p " + fileDir);
    string divFilePath = fileDir + "." + filename;
    if (!Tools::fileExists(divFilePath)) {
        // 存入分割条目的信息
        json_object *fileList = json_object_new_array();

        for (int i = 0; i * pieceSize - 1 < size; i++) {
            stringstream ii;
            ii << i + 1;

            string tName = "." + filename + ii.str() + ".tmp";
            int tSize = size;
            char range[64];
            int fromSize = i * pieceSize;
            int toSize = (i + 1) * pieceSize - 1;
            toSize = toSize > size ? size : toSize;

            sprintf(range, "%d-%d", fromSize, toSize);
            string rangeStr = range;
            Tools::log("range:" + rangeStr);

            json_object *tFileObject = json_object_new_object();
            json_object_object_add(tFileObject, "name", json_object_new_string(tName.data()));
            json_object_object_add(tFileObject, "range", json_object_new_string(range));
            json_object_object_add(tFileObject, "size", json_object_new_int64(tSize));
            json_object_object_add(tFileObject, "start", json_object_new_boolean(false));
            json_object_object_add(tFileObject, "success", json_object_new_boolean(false));

            Tools::log(json_object_get_string(tFileObject));

            json_object_array_add(fileList, json_object_get(tFileObject));

            json_object_put(tFileObject);
        }
        Tools::log(json_object_get_string(fileList));
        json_object_to_file((char *) divFilePath.data(), fileList);
        json_object_put(fileList);
    }


    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    localFilePath = USERDATA + localFilePath;
    string accessToken = Tools::getData("accessToken");

    json_object *fileListJson = json_object_from_file(divFilePath.data());

    string curlPids = Tools::runCommand("pidof curl");
    char *s = (char*)curlPids.data();

    Tools::log("curl pid:"+curlPids);
    const char *sep = " "; //可按多个字符来分割
    char *p;
    p = strtok(s, sep);
    int curlTreadCount = 0;
    while (p) {
        //printf("%s ", p);
        p = strtok(NULL, sep);
        curlTreadCount++;
    }

    int length = json_object_array_length(fileListJson);

    for (int j = 0; j < length; ++j) {
        json_object *divItem = json_object_array_get_idx(fileListJson, j);
        string name = json_object_get_string(json_object_object_get(divItem, "name"));
        string range = json_object_get_string(json_object_object_get(divItem, "range"));
        int size = json_object_get_int(json_object_object_get(divItem, "size"));
        bool start = json_object_get_boolean(json_object_object_get(divItem, "start"));
        bool success = json_object_get_boolean(json_object_object_get(divItem, "success"));

        string strUrl =
                "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
                pcsFilePath;
        string command =
                "curl -sSLk --retry 3 --range " + range + " -A '' -o \"" + fileDir+name + "\" " + "\"" + strUrl + "\"";
        Tools::runCommand(command);
    }

    return "";
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
