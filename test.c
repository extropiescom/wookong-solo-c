#include <stdio.h>
#include "src/solo.h"

//GetAddress input
//{"coin":"BTC", "adjust":0, showonscreen":true}
char* address = "{\"coin\":\"ETH\", \"adjust\":0, \"showonscreen\":true}";

void test_get()
{

}

void test_post()
{
   
}
void main()
{
    unsigned char v[100] = { 0 };
    int rv = 0;
    printf("hello solo\n");
    char *p = NULL;

    p = get(URL_GET_COUNT,0);
    printf("URL_GET_COUNT return: %s\n",p);

    p = get(URL_GET_INFO,0);
    printf("URL_GET_INFO return: %s\n",p);

    p = post(URL_POST_ADDRESS,0,address);
    printf("URL_POST_ADDRESS return: %s\n",p);
}