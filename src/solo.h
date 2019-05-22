
#define URL_GET_COUNT           "count"
#define URL_GET_INFO            "info"
#define URL_GET_MODIFYPIN       "modifypin"
#define URL_GET_GENERATE        "generate"
#define URL_GET_FORMAT          "format"
#define URL_GET_IMPORT          "import"
#define URL_GET_REBOOT          "reboot"


#define URL_POST_ADDRESS        "address"
#define URL_POST_SIGN           "sign"
#define URL_POST_XRPPUB         "xrppub"
#define URL_POST_UPDATE         "update"
#define URL_POST_VERIFY         "verify"

#define GET_FUNC_AMOUNT         7
#define POST_FUNC_AMOUNT        5

#define ERR_CODE_BADREQUEST          400
#define ERR_CODE_NOACCESS            403 
#define ERR_CODE_NOTFOUND            404
#define ERR_CODE_INTERNAL            500


char* post(char* url, int port, char* data);

char* get(char* url,int port);


