#include "Tools.h"

std::string
Tools::runCommand(std::string command) {
    std::string output = "";
    router::PluginTools::sCallSystem(command, output);
    return output;
}

int
Tools::saveData(const std::string key, const std::string value) {
    return router::PluginTools::saveData(key, value);
}

std::string
Tools::getData(std::string key) {
    std::string value = "";
    router::PluginTools::getData(key, value);
    return value;
}

std::string
Tools::getParams(const std::string params) {
    const char *ch = params.data();
    struct json_object *jsonObject = NULL;
    jsonObject = json_tokener_parse(ch);
    std::string data = "";
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        jsonObject = json_object_object_get(jsonObject, "data");
        data = json_object_get_string(jsonObject);
    }
    json_object_put(jsonObject);
    return data;
}

std::string
Tools::getParamsByKey(const std::string params, std::string key) {
    const char *ch = params.data();
    struct json_object *jsonObject = NULL;
    jsonObject = json_tokener_parse(ch);
    std::string data = "";
    if ((long) jsonObject > 0) {/**Json格式无错误**/
        jsonObject = json_object_object_get(jsonObject, key.data());
        data = json_object_get_string(jsonObject);
    }
    json_object_put(jsonObject);
    return data;
}

size_t req_reply(void *ptr, size_t size, size_t nmemb, void *stream) {
    //cout << "----->reply" << endl;
    string *str = (string *) stream;
    cout << *str << endl;
    (*str).append((char *) ptr, size * nmemb);
    return size * nmemb;
}

std::string
Tools::getUrl(const std::string url) {
    std::string response;
    // init curl
    CURL *curl = curl_easy_init();
    // res code
    //CURLcode res;
    if (curl) {
        // set params
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3); // set transport and time out time
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        //res = curl_easy_perform(curl);
        curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return response;
}

std::string
Tools::postUrl(const std::string url, const std::string postParams) {
    std::string response = 0;

    // init curl
    CURL *curl = curl_easy_init();
    // res code
    //CURLcode res;
    if (curl) {
        // set params
        curl_easy_setopt(curl, CURLOPT_POST, 1); // post req
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // url
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams.c_str()); // params
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
        // start req
        //res = curl_easy_perform(curl);
        curl_easy_perform(curl);
    }
    // release curl
    curl_easy_cleanup(curl);
    return response;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int
Tools::download(std::string downloadUrl, std::string savePath) {

    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(savePath.data(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, downloadUrl.data());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    } else {
        printf("!!!curl init failed\n");
        return -1;
    }
    return 0;
}


int
Tools::upload(std::string uploadUrl, std::string localFilePath) {
    std::string response = 0;
    CURL *curl;
    CURLcode res;
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    curl_global_init(CURL_GLOBAL_ALL);
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "file",
                 CURLFORM_FILE, localFilePath.data(),
                 CURLFORM_END);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, uploadUrl.data());
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
    Tools::runCommand("echo \"" + response + "\">>/sync.log");
    return 0;
}


unsigned char ToHex(unsigned char x) {
    return x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x) {
    unsigned char y;
    if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if (x >= '0' && x <= '9') y = x - '0';
    else assert(0);
    return y;
}

std::string Tools::urlEncode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char) str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += ToHex((unsigned char) str[i] >> 4);
            strTemp += ToHex((unsigned char) str[i] % 16);
        }
    }
    return strTemp;
}

std::string Tools::urlDecode(const std::string &str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (str[i] == '+') strTemp += ' ';
        else if (str[i] == '%') {
            assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char) str[++i]);
            unsigned char low = FromHex((unsigned char) str[++i]);
            strTemp += high * 16 + low;
        } else strTemp += str[i];
    }
    return strTemp;
}

void Tools::log(std::string logStr) {
    time_t t = time(0);
    char dateTime[64];
    char dateTime2[64];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S : ", localtime(&t));
    strftime(dateTime2, sizeof(dateTime2), "%Y-%m-%d", localtime(&t));
    std::string dateFormat = dateTime;
    std::string logPath = dateTime2;
    Tools::runCommand("mkdir -p /logs");
    Tools::runCommand("echo \"" + dateFormat + logStr + "\">>/logs/" + logPath + ".log");
}

#define N 6

void Tools::fileSplit(std::string filePath)
{
    string filename = filePath.substr(filePath.find_last_of('/') + 1);//获取文件后缀
    string fileDir = filePath.substr(0,filePath.find_last_of('/') - 1);//获取文件后缀

    FILE *fsrc = fopen(filePath.data(), "rb");  // 源文件
    std::string divName = filePath+"_div.txt";
    FILE *div = fopen(divName.data(),"w");  // 存入分割条目的信息

    if (fsrc == NULL || div == NULL)
    {
        perror("打开错误：");
        return;
    }
    fseek(fsrc, 0, SEEK_END);
    int fLen = ftell(fsrc);  // 文件长度
    //printf("文件长度：%d\n", fLen);
    int blockLen = fLen / N;   // 每一块的长度
    //printf("blockLen:%d\n", blockLen);
    FILE *ftmp;  // 临时文件，
    for (int i = 0; i < N; i++)  // 按块分割
    {
        char tName[20];
        char tdir[60] = "/";
        sprintf(tName, "%s%d.tmp", filename.data(),i+1);//生成文件名
        strcat(tdir, tName);  //产生临时目录
        //printf("%s\n", tdir);
        ftmp = fopen(tdir, "wb");  // 生成临时文件
        if (ftmp == NULL)
        {
            perror("产生文件出错：");
            break;
        }
        fputs(tdir, div); // 写入文件名
        fputc('\n',div);

        int offset = i*blockLen; //计算偏移量
        fseek(fsrc, offset, SEEK_SET);
        int count = 0;  //统计写入ftmp的数量
        if (i == N - 1) blockLen = fLen - blockLen*(N - 1);  //最后一块的长度
        while (count<blockLen && !feof(fsrc))
        {

            fputc(fgetc(fsrc),ftmp);
            count++;
        }
        printf("count:%d\n", count);
        fclose(ftmp);
    }

    fclose(fsrc);
    fclose(div);

}

void Tools::fileMerge(std::string filePath) {


    FILE *fdest = fopen(filePath.data(), "wb"); //合并生成的文件
    std::string divName = filePath+"_div.txt";
    FILE *div = fopen(divName.data(), "r");  // 读取已分割部分的目录
    if ( fdest == NULL || div == NULL)
    {
        perror("打开文件出错");
        return;
    }

    char tempName[60];
    FILE *tempFile;
    // 循环读出temp文件路径，并进行文件的合并
    while (fgets(tempName, 60, div)) // fgets读取到字符串时返回非0，否则返回0
    {

        tempName[strlen(tempName)-1] = '\0'; // 去掉最后一个\n
        tempFile = fopen(tempName, "rb");
        if (tempFile == NULL)
        {
            //printf("打开文件%s失败,", tempName);
            perror("出错原因");
            return;
        }


        //printf("正在合并%s到新文件\n",tempName);
        // 修正后的代码
        int ch = fgetc(tempFile);//在用feof前先读取一个字符

        //判断你上面读取的ch是否为结束符，feof在读取到EOF才会返回1，
        //即若ch为EOF，则while循环不会进入，而当ch为EOF的前一次读取时，while循环仍会继续。
        while (!feof(tempFile))
        {
            fputc(ch, fdest); //写入字符
            ch = fgetc(tempFile);//读取下一个字符 如果是EOF,那么进入下一次循环前就会停止
        }

        fclose(tempFile);
    }
    fclose(fdest);
    fclose(div);

}

