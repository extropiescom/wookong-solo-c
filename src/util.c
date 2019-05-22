#include <string.h>
#include <stdio.h>
#include "core/PA_EWallet.h"
#include "cJSON/cJSON.h"
#include "util.h"


void ByteToHexStr(const unsigned char* source, char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;
 
    for (i = 0; i < sourceLen; i++)
    {
        highByte = source[i] >> 4;
        lowByte = source[i] & 0x0f ;
 
        highByte += 0x30;
 
        if (highByte > 0x39)
                dest[i * 2] = highByte + 0x07;
        else
                dest[i * 2] = highByte;
 
        lowByte += 0x30;
        if (lowByte > 0x39)
            dest[i * 2 + 1] = lowByte + 0x07;
        else
            dest[i * 2 + 1] = lowByte;
    }
    return ;
}

void HexStrToBytes(const char* source, unsigned char* dest, int sourceLen)
{
    short i;
    unsigned char highByte, lowByte;
    
    for (i = 0; i < sourceLen; i += 2)
    {
        highByte = toupper(source[i]);
        lowByte  = toupper(source[i + 1]);
 
        if (highByte > 0x39)
            highByte -= 0x37;
        else
            highByte -= 0x30;
 
        if (lowByte > 0x39)
            lowByte -= 0x37;
        else
            lowByte -= 0x30;
 
        dest[i / 2] = (highByte << 4) | lowByte;
    }
    return;
}
//ec098504a817c800825208943535353535353535353535353535353535353535880de0b6b3a764000080018080
int parse_tx(char *data, unsigned char* tx_bytes)
{
    char *strTx = NULL;
    int len = 0;
    cJSON *obj = cJSON_Parse(data);
    cJSON *item = cJSON_GetObjectItem(obj, "tx");
    strTx = cJSON_GetStringValue(item);
    len = strlen(strTx);
    //TODO hexstring to bytes
    HexStrToBytes(strTx,tx_bytes,strlen(strTx));
    return strlen(strTx)/2;
}

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