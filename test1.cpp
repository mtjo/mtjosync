#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#define POSTURL "https://pcs.baidu.com/rest/2.0/pcs/file?method=upload&access_token=21.e8d354bb2fd2c7d341d2db6685857963.2592000.1472779881.2738093065-2339858&path=%2fapps%2fmtjo%2f1231.xlsx"
 
int main(int argc, char *argv[])
{
    printf("%d\n",argc);//格式化输出
    while(argc)//当(统计参数个数)
    printf("%s\n",argv[--argc]);//格式化输出


printf("%s\n",argv[1]);
printf("%s\n",POSTURL);

char buffer[1024];
snprintf("%s%s", sizeof(buffer), POSTURL, POSTURL);
printf("%s\n",buffer);
  CURL *curl;
  CURLcode res;
 
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* Fill in the file upload field */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "message",
               CURLFORM_COPYCONTENTS, "hello",
               CURLFORM_END);
 
  /* Fill in the filename field */ 
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_FILE,"/home/mtjo/下载/附件2测试案例报告.xlsx",
               CURLFORM_CONTENTTYPE, "application/vnd.ms-excel",
               CURLFORM_END);
 
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, POSTURL);
    if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
      /* only disable 100-continue header if explicitly requested */ 
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
  return 0;
}
