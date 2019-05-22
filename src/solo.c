#include "solo.h"
#include <string.h>
#include <stdio.h>
#include "wrapper.h"
#include "util.h"
#include "PA_EWallet.h"

char *post(char *url, int port, char *data)
{
    const static char *post_urls[] = {
        URL_POST_ADDRESS,
        URL_POST_SIGN,
        URL_POST_XRPPUB,
        URL_POST_UPDATE,
        URL_POST_VERIFY};
    char *(*post_funcs[])(int, char *) = {post_address, post_sign, post_xrppub, post_update, post_verify};
    char *p = NULL;
    int i = 0;
    
    if (url == NULL)
    {
        p = create_code(ERR_CODE_BADREQUEST);
        return p;
    }
    do
    {
        if (!strcmp(url, post_urls[i]))
        {
            p = post_funcs[i](port, data);
            break;
        }
        i++;
    } while (i < GET_FUNC_AMOUNT);

    if (i == GET_FUNC_AMOUNT)
    {
        p = create_code(ERR_CODE_NOTFOUND);
        return p;
    }

    return p;
}

char *get(char *url, int port)
{
    const static char *get_urls[] = {
        URL_GET_COUNT,
        URL_GET_INFO,
        URL_GET_MODIFYPIN,
        URL_GET_GENERATE,
        URL_GET_FORMAT,
        URL_GET_IMPORT,
        URL_GET_REBOOT};
    char *p = NULL;
    int i = 0;
    char *(*get_funcs[])(int) = {get_count, get_info, get_modifypin, get_generate, get_format, get_import, get_reboot};

    if (url == NULL)
    {
        p = create_code(ERR_CODE_BADREQUEST);
        return p;
    }

    do
    {
        if (!strcmp(url, get_urls[i]))
        {
            p = get_funcs[i](port);
            break;
        }
        i++;
    } while (i < GET_FUNC_AMOUNT);

    if (i == GET_FUNC_AMOUNT)
    {
        p = create_code(ERR_CODE_NOTFOUND);
        return p;
    }

    return p;
}