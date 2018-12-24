#include "BaiduPcs.h"


std::string BaiduPcs::getPcsQuota() {
    std::string accessToken = Tools::getData("access_token");
    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/quota?method=info&access_token=" + accessToken;
    return Tools::getUrl(strUrl);
}

std::string BaiduPcs::getPcsFileMeta(std::string path) {

    std::string accessToken = Tools::getData("access_token");
    const std::string strUrl =
            "https://pcs.baidu.com/rest/2.0/pcs/file?method=meta&access_token=" + accessToken + "&path=" + path;
    return Tools::getUrl(strUrl);

}


std::string BaiduPcs::getPcsFileList(std::string pcsPath, std::string limit1, std::string limit2) {
    std::string accessToken = Tools::getData("access_token");
    const std::string strUrl = "https://pcs.baidu.com/rest/2.0/pcs/file?method=list&access_token=" + accessToken +
                               "&path=" + pcsPath +
                               "&limit=" + limit1 + "-" + limit2 +
                               "&by=name&order=asc";
    return Tools::getUrl(strUrl);
}


std::string BaiduPcs::downloadPcsFile(std::string pcsFilePath, std::string localFilePath) {
    std::string accessToken = Tools::getData("access_token");

    std::string url =
            "https://d.pcs.baidu.com/rest/2.0/pcs/file?method=download&access_token=" + accessToken + "&path=" +
            pcsFilePath;
    Tools::download(url, localFilePath);
    return "";
}
