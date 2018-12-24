#include "BaiduPcs.h"


std::string BaiduPcs::getPcsQuota() {
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" + accessToken;
    return Tools::getUrl(strUrl);
}

std::string BaiduPcs::getPcsFileMeta(std::string path) {
    path=Tools::urlEncode(PCSROOT+path);
    std::string accessToken = Tools::getData("accessToken");
    const std::string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=meta&access_token=" + accessToken + "&path=" + path;
    Tools::runCommand("echo "+strUrl+ ">>sync.log");
    return Tools::getUrl(strUrl);

}


std::string BaiduPcs::getPcsFileList(std::string pcsPath, int start,int end) {
    pcsPath=Tools::urlEncode(PCSROOT+pcsPath);
    std::string accessToken = Tools::getData("accessToken");

    std::stringstream ss;
    ss<<start;
    std::string startStr = ss.str();
    ss<<end;
    std::string endStr = ss.str();

    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/file?method=list&access_token=" + accessToken +
                               "&path=" + pcsPath +
                               "&limit=" + startStr + "-" + endStr +
                               "&by=name&order=asc";
    Tools::runCommand("echo "+strUrl+ ">>sync.log");
    return Tools::getUrl(strUrl);
}


std::string BaiduPcs::downloadPcsFile(std::string pcsFilePath, std::string localFilePath) {
    pcsFilePath=Tools::urlEncode(PCSROOT+pcsFilePath);
    std::string accessToken = Tools::getData("accessToken");
    std::string strUrl =
            "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
            pcsFilePath;
    Tools::runCommand("echo "+strUrl+ ">>sync.log");
    Tools::download(strUrl, localFilePath);
    return "";
}
