#pragma once
#include <string>
#include "json/json.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Tools.h"
#include <sstream>
#include "json/json.h"


using std::string;
using std::stringstream;
using namespace std;

#define PCSROOT "/apps/mtjo"
#define USERDATA "/userdata"
class BaiduPcs {

public:
    static std::string getPcsQuota();
    static std::string mkdir(std::string pcsPath);
    static std::string getPcsFileMeta(std::string pcsPath);
    static std::string getPcsFileList(std::string pcsPath,int start,int end);
    static std::string uploadFile2Pcs(std::string localFilePath,std::string pcsFilePath,std::string type);
    static std::string movePcsFile(std::string pcsFromPath,std::string pcsToPath);

    static std::string mUpload2Pcs(std::string pcsPath,std::string limit1,std::string limit2);

    static std::string rapidUploadFile2Pcs(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string downloadPcsFile(std::string pcsFilePath, std::string localFilePath);
    static std::string burstDownloadPcsFile(std::string pcsFilePath, std::string localFilePath,int treadCount);
    static std::string deletePcsFile(std::string pcsPath,std::string limit1,std::string limit2);

    static std::string checkPcsPath(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string checkAndCreatePCSdir(std::string pcsPath,std::string limit1,std::string limit2);
};
