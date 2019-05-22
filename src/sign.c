#include <string.h>
#include "cJSON/cJSON.h"
#include "util.h"
#include "PA_EWallet.h"
#include "sign.h"

char *sign_tx(int coin, char *tx)
{
    int iRtn = -1;

    void *pPAEWContext = 0;
    size_t nDevCount;

    PAEW_DevInfo devInfo;
    size_t i = 0;
 
    unsigned char pbAddressData[PAEW_COIN_ADDRESS_MAX_LEN] = {0};
    size_t nAddressDataLen = 0;

    unsigned char pbTXSig[PAEW_SIG_MAX_LEN] = {0};
    size_t nTXSigLen = PAEW_SIG_MAX_LEN;
    unsigned char strTXSig[PAEW_SIG_MAX_LEN*2] = {0};

    unsigned char pbTX[PAEW_TX_MAX_LEN] = {0};

    unsigned int path[6] = {0};
    unsigned char strPath[130] = {0};

    int txLen = 0;

    fethc_path(coin, path);
    txLen = parse_tx(tx,pbTX);

    iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
    if (iRtn != PAEW_RET_SUCCESS)
    {
        iRtn = -1;
        goto END;
    }

    iRtn = PAEW_GetDevInfo(pPAEWContext, 0, PAEW_DEV_INFOTYPE_COS_TYPE, &devInfo);
    if (iRtn != PAEW_RET_SUCCESS)
    {
        iRtn = -2;
        goto END;
    }
    iRtn = PAEW_DeriveTradeAddress(pPAEWContext, i, coin, path, sizeof(path) / sizeof(path[0]));
    if (iRtn != PAEW_RET_SUCCESS)
    {
        goto END;
    }
    nAddressDataLen = sizeof(pbAddressData);
    iRtn = PAEW_GetTradeAddress(pPAEWContext, i, coin, 0, pbAddressData, &nAddressDataLen);
    if (iRtn != PAEW_RET_SUCCESS)
    {
        iRtn = -9;
        goto END;
    }

    switch (coin)
    {
    case PAEW_COIN_TYPE_ETH:
        nTXSigLen = sizeof(pbTXSig);
        iRtn = PAEW_ETH_TXSign(pPAEWContext, i, pbTX, txLen, pbTXSig, &nTXSigLen);
        
        if (iRtn != PAEW_RET_SUCCESS)
        {
            goto END;
        }
        break;
    case PAEW_COIN_TYPE_ETC:
        nTXSigLen = sizeof(pbTXSig);
        iRtn = PAEW_ETC_TXSign(pPAEWContext, i, pbTX, txLen, pbTXSig, &nTXSigLen);
        if (iRtn != PAEW_RET_SUCCESS)
        {
            goto END;
        }
        break;
    case PAEW_COIN_TYPE_EOS:
        nTXSigLen = sizeof(pbTXSig);
        iRtn = PAEW_EOS_TXSign(pPAEWContext, i, pbTX, txLen, pbTXSig, &nTXSigLen);
        if (iRtn != PAEW_RET_SUCCESS)
        {
            goto END;
        }
        break;
    case PAEW_COIN_TYPE_CYB:
        nTXSigLen = sizeof(pbTXSig);
        iRtn = PAEW_CYB_TXSign(pPAEWContext, i, pbTX, txLen, pbTXSig, &nTXSigLen);
        if (iRtn != PAEW_RET_SUCCESS)
        {
            iRtn = -4;
            goto END;
        }
    case PAEW_COIN_TYPE_XRP:
        nTXSigLen = sizeof(pbTXSig);
        iRtn = PAEW_XRP_TXSign(pPAEWContext, i, pbTX, txLen, pbTXSig, &nTXSigLen);
        if (iRtn != PAEW_RET_SUCCESS)
        {
            iRtn = -4;
            goto END;
        }
        break;

    default:
        break;
    }

END:
    PAEW_FreeContext(pPAEWContext);
    if (iRtn == 0)
    {
        ByteToHexStr(pbTXSig,strTXSig,nTXSigLen);
        return create_code_result_str(0, "signature", strTXSig);
    }
    else
    {
        return create_code(iRtn);
    }
}
