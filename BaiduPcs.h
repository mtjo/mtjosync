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
using std::string;
using std::stringstream;

#define PCSROOT "/apps/mtjo"
class BaiduPcs {

public:
    static std::string getPcsQuota();
    static std::string getPcsFileMeta(std::string path);
    static std::string getPcsFileList(std::string pcsPath,int start,int end);
    static std::string mUpload2Pcs(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string uploadFile2Pcs(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string rapidUploadFile2Pcs(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string downloadPcsFile(std::string pcsFilePath, std::string localFilePath);
    static std::string deletePcsFile(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string movePcsFile(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string checkPcsPath(std::string pcsPath,std::string limit1,std::string limit2);
    static std::string checkAndCreatePCSdir(std::string pcsPath,std::string limit1,std::string limit2);
};
