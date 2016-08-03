#include "HCUrl.h"

#include <string.h>

using namespace OSLib;

class MyCCUrl : public CCUrl
{
protected:
    int process( void* data, size_t size, size_t nmemb )
    {
        //printf( "\n================entering size=%d================\n", size * nmemb );

        printf( "%s", (char*)data );
        return size * nmemb;
    }    
};


void testCCUrlDoHttpPost( const char* username, const char* password );
void testCCUrlDoHttpGet( const char* name, const char* age );
void testGlobalCleanup( const char* username, const char* password );
void testCCUrlUploadFile( const char* filename );

int main( int argc, char* argv[] )
{
//     if( argc < 3 )        

//     {

//         printf( "usage:%s username password\n", argv[0] );

//         return 0;

//     }

//     testCCUrlDoHttpPost( argv[1], argv[2] );



    if( argc < 3 )        
    {
        printf( "usage:%s name age\n", argv[0] );
        return 0;
    }
    testCCUrlDoHttpGet( argv[1], argv[2] );

//     if( argc < 3 )        

//     {

//         printf( "usage:%s username password\n", argv[0] );

//         return 0;

//     }

//     testGlobalCleanup( argv[1], argv[2] );

    
//     if( argc < 2 )        

//     {

//         printf( "usage:%s filename\n", argv[0] );

//         return 0;

//     }

//     testCCUrlUploadFile( argv[1] );

    
    return 0;
}


void testCCUrlDoHttpPost( const char* username, const char* password )
{
    MyCCUrl cu;
    cu.init();
    char data[1024];
    sprintf( data, "id=%s&passwd=%s", username, password );
    char buf[1024 * 1024];
    char* url = "http://forum.byr.edu.cn/wForum/logon.php";
    cu.doHttpPost( url, data, 10 );
} 

void testCCUrlDoHttpGet( const char* name, const char* age )
{
    MyCCUrl cu;
    cu.init();
    char data[1024];
    sprintf( data, "name=%s&age=%s", name, age );
    char buf[1024 * 1024];
    char url[1024];
    sprintf( url, "http://192.168.3.200:8080/action.get.php", name, age );
    cu.doHttpGet( url, data, 10, 1 );
} 

// void testCCUrlUploadFile( const char* filename )

// {

//     CCUrl cu;

//     cu.init();

//     char url[1024];

//     sprintf( url, "http://192.168.3.200:8080/%s", filename );

//     cu.uploadFile( url, filename );

// } 



void testGlobalCleanup( const char* username, const char* password )
{
    for ( int i = 0; i < 10; i ++ )
    {
        MyCCUrl* cu = new MyCCUrl;
        cu->init();
        char data[1024];
        sprintf( data, "id=%s&passwd=%s", username, password );
        char* url = "http://forum.byr.edu.cn/wForum/logon.php";
        cu->doHttpPost( url, data, 10 );
    
        printf( "\n\n\n=============================================\n\n\n" );
    
        MyCCUrl* cu2 = new MyCCUrl;
        
        cu2->init();
        sprintf( data, "name=%s&age=%s", username, password );
        url = "http://192.168.3.200:8080/action.get.php";
        
        delete cu;    //check after deleting cu, whether cu2 is ok 


        cu2->doHttpGet( url, data, 10 );
        
        delete cu2;
    }

    for ( int i = 0; i < 10; i ++ )
    {
        MyCCUrl* cu = new MyCCUrl;
        cu->init();
        char data[1024];
        char* url = 0;
        
    
        printf( "\n\n\n=============================================\n\n\n" );
    
        MyCCUrl* cu2 = new MyCCUrl;
        
        cu2->init();
        sprintf( data, "name=%s&age=%s", username, password );
        url = "http://192.168.3.200:8080/action.get.php";
        cu2->doHttpGet( url, data, 10 );
        delete cu2;
        
        url = "http://forum.byr.edu.cn/wForum/logon.php";
        sprintf( data, "id=%s&passwd=%s", username, password );
        cu->doHttpPost( url, data, 10 );
        delete cu;    //check after deleting cu, whether cu2 is ok 

    }
}
