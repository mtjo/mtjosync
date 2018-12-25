#include "BaiduPcs.h"

std::string BaiduPcs::getPcsQuota() {
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" + accessToken;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}

std::string BaiduPcs::mkdir(std::string pcsPath) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=mkdir&access_token=" + accessToken + "&path=" + pcsPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}

std::string BaiduPcs::getPcsFileMeta(std::string pcsPath) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=meta&access_token=" + accessToken + "&path=" + pcsPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);

}


std::string BaiduPcs::getPcsFileList(std::string pcsPath, int start, int end) {
    pcsPath = Tools::urlEncode(PCSROOT + pcsPath);
    std::string accessToken = Tools::getData("accessToken");
    std::stringstream ss;
    ss << start;
    std::string startStr = ss.str();
    ss << end;
    std::string endStr = ss.str();
    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/file?method=list&access_token=" + accessToken +
                               "&path=" + pcsPath +
                               "&limit=" + startStr + "-" + endStr +
                               "&by=name&order=asc";
    Tools::log(strUrl);

    return Tools::getUrl(strUrl);
}


std::string BaiduPcs::downloadPcsFile(std::string pcsFilePath, std::string localFilePath) {
    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    std::string accessToken = Tools::getData("accessToken");
    std::string strUrl =
            "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
            pcsFilePath;
    std::string command =
            "curl -sSLk -R --retry 3 -Y 1 -y 60 -A '' -o \"" + localFilePath + "\" " + "\"" + strUrl + "\"";
    Tools::log(strUrl);
    return Tools::runCommand(command);
    //Tools::download(strUrl, localFilePath);
}

std::string BaiduPcs::uploadFile2Pcs(std::string localFilePath, std::string pcsFilePath, std::string ondup) {
    pcsFilePath = Tools::urlEncode(PCSROOT + pcsFilePath);
    std::string accessToken = Tools::getData("accessToken");
    std::string strUrl =
            "https://c.pcs.baidu.com/rest/2.0/pcs/file?method=upload&access_token=" + accessToken +
            "&path=" + pcsFilePath +
            "&ondup=" + ondup;
    Tools::log(strUrl);
    std::string command =
            "curl -sSLk --retry $retrytimes -A '' -F \"file=@\"" + localFilePath + "\"" + "\"" + strUrl + "\"";
    //Tools::upload(strUrl, localFilePath);
    return Tools::runCommand(command);

}

std::string BaiduPcs::movePcsFile(std::string pcsFromPath, std::string pcsToPath) {
    pcsFromPath = Tools::urlEncode(PCSROOT + pcsFromPath);
    pcsToPath = Tools::urlEncode(PCSROOT + pcsToPath);
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=move&access_token=" + accessToken +
            "&from=" + pcsFromPath +
            "&to=" + pcsToPath;
    Tools::log(strUrl);
    return Tools::getUrl(strUrl);
}
