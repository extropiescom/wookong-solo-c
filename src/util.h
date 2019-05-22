#include "core/PA_EWallet.h"

#define MAX_RESULT_LEN      10240
#define CODE_RESULT_LEN     1024
#define COIN_AMOUNT         10
#define COIN_NOT_FOUND      -1

/*const uint32_t puiBTCPath[] = {0, 0x8000002c, 0x80000000, 0x80000000, 0x00000000, 0x00000000};
const uint32_t puiETHPath[] = {0, 0x8000002c, 0x8000003c, 0x80000000, 0x00000000, 0x00000000};
const uint32_t puiCYBPath[] = {0, 0x8000002c, 0x00000001, 0x80000000, 0x00000000, 0x00000000};
const uint32_t puiNEOPath[] = {0, 0x8000002C, 0x80000378, 0x80000000, 0x00000000, 0x00000000};
const uint32_t puiEOSPath[] = {0, 0x8000002C, 0x800000c2, 0x80000000, 0x00000000, 0x00000000};
const uint32_t puiETCPath[] = {0, 0x8000002C, 0x8000003d, 0x80000000, 0x00000000, 0x00000000};*/
int parse_showonscreen(char* data);
int parse_coin(char* json);
void fethc_path(int coin, unsigned int* path);
char *create_code(int code);
char *create_code_result_int(int code, char *tag, int value);
char *create_code_result_str(int code, char *tag, char *str);
char *create_devinfo(int code, PAEW_DevInfo devInfo);