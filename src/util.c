#include <string.h>
#include <stdio.h>
#include "core/PA_EWallet.h"
#include "cJSON/cJSON.h"
#include "util.h"

int parse_showonscreen(char* data)
{
    cJSON *obj = cJSON_Parse(data);
    return cJSON_GetObjectItem(obj, "showonscreen")->valueint;
}
int parse_coin(char *json)
{
    char *coin = NULL;
    const char *coins[] = {"BTC", "ETH", "CYB", "EOS", "LTC", "NEO", "ETC", "BTC_WIT", "BTC_SIGWIT", "XRP", "USDT"};
    int index = 0;
    cJSON *obj = cJSON_Parse(json);
    cJSON *item = cJSON_GetObjectItem(obj, "coin");
    coin = cJSON_GetStringValue(item);
    printf("parse coin = %s\n", coin);
    while (index < COIN_AMOUNT)
    {
        if (!strcmp(coins[index], coin))
            break;
        index++;
    }
    if (index == COIN_AMOUNT)
        index = COIN_NOT_FOUND;
    printf("parse coin index= %d\n", index);
    return index;
}
void fethc_path(int coin, unsigned int* path)
{
    unsigned int paths[11][6] = {
        {0, 0x8000002c, 0x80000000, 0x80000000, 0x00000000, 0x00000000}, //BTC
        {0, 0x8000002c, 0x8000003c, 0x80000000, 0x00000000, 0x00000000}, //ETH
        {0, 0x8000002c, 0x00000001, 0x80000000, 0x00000000, 0x0000000},  //CYB
        {0, 0x8000002C, 0x800000c2, 0x80000000, 0x00000000, 0x00000000}, //EOS
        {0, 0, 0, 0, 0, 0},//LTC
        {0, 0x8000002C, 0x80000378, 0x80000000, 0x00000000, 0x00000000}, //NEO
        {0, 0x8000002C, 0x8000003d, 0x80000000, 0x00000000, 0x00000000}, //ETC
        {0,0,0,0,0,0},//WIT
        {0,0,0,0,0,0},//SGIWIT
        {0,0,0,0,0,0},//XRP
        {0,0,0,0,0,0}//USDT
    };
    memcpy(path, paths[coin],24);
}
char *create_code(int code)
{
    static char out[CODE_RESULT_LEN] = {0};
    char *p = NULL;
    cJSON *root = NULL;

    memset(out, 0, CODE_RESULT_LEN);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "code", code);

    p = cJSON_Print(root);
    strcpy(out, p);
    return out;
}
char *create_code_result_int(int code, char *tag, int value)
{
    static char out[CODE_RESULT_LEN] = {0};
    char *p = NULL;
    cJSON *root = NULL;
    cJSON *result = NULL;

    memset(out, 0, CODE_RESULT_LEN);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "code", code);
    cJSON_AddItemToObject(root, "result", result = cJSON_CreateObject());
    cJSON_AddNumberToObject(result, tag, value);

    p = cJSON_Print(root);
    strcpy(out, p);
    return out;
}
char *create_code_result_str(int code, char *tag, char *str)
{
    static char out[CODE_RESULT_LEN] = {0};
    char *p = NULL;
    cJSON *root = NULL;
    cJSON *result = NULL;

    memset(out, 0, CODE_RESULT_LEN);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "code", code);
    cJSON_AddItemToObject(root, "result", result = cJSON_CreateObject());
    cJSON_AddStringToObject(result, tag, str);

    p = cJSON_Print(root);
    strcpy(out, p);
    return out;
}

void array_char2int(unsigned char *c, int *i, int len)
{
    int j = 0;
    while (j < len)
    {
        i[j] = 0x000000ff & ((int)c[j]);
        j++;
    }
}

char *create_devinfo(int code, PAEW_DevInfo devInfo)
{
    static char out[MAX_RESULT_LEN] = {0};
    char *p = NULL;
    cJSON *root = NULL;
    cJSON *result = NULL;
    int iBuffer[4] = {0};

    memset(out, 0, MAX_RESULT_LEN);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "code", code);
    cJSON_AddItemToObject(root, "result", result = cJSON_CreateObject());
    cJSON_AddNumberToObject(result, "ucPINState", devInfo.ucPINState);
    cJSON_AddNumberToObject(result, "ucCOSType", devInfo.ucCOSType);
    cJSON_AddNumberToObject(result, "ucChainType", devInfo.ucChainType);
    cJSON_AddStringToObject(result, "pbSerialNumber", devInfo.pbSerialNumber);
    array_char2int(devInfo.pbCOSVersion, iBuffer, 4);
    cJSON_AddItemToObject(result, "pbCOSVersion", cJSON_CreateIntArray(iBuffer, 4));
    cJSON_AddNumberToObject(result, "ucLifeCycle", devInfo.ucLifeCycle);
    cJSON_AddNumberToObject(result, "nLcdState", devInfo.nLcdState);
    array_char2int(devInfo.pbSessKeyHash, iBuffer, 4);
    cJSON_AddItemToObject(result, "pbSessKeyHash", cJSON_CreateIntArray(iBuffer, 4));
    cJSON_AddNumberToObject(result, "nN", devInfo.nN);
    cJSON_AddNumberToObject(result, "nT", devInfo.nT);
    array_char2int(devInfo.pbBLEVersion, iBuffer, 4);
    cJSON_AddItemToObject(result, "pbBLEVersion", cJSON_CreateIntArray(iBuffer, 4));

    p = cJSON_Print(root);
    strcpy(out, p);
    return out;
}