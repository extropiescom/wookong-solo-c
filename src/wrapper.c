#include <stdio.h>
#include <string.h>
#include "PA_EWallet.h"
#include "util.h"
#include "sign.h"
#include "wrapper.h"


char *get_count(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code_result_int(iRtn, "count", nDevCount);
}

char *get_info(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	size_t i = 0;
	PAEW_DevInfo devInfo;
	uint32_t nDevInfoType = 0;
	unsigned char *szDeviceName = NULL;

	if (szDeviceName)
	{
		iRtn = PAEW_InitContextWithDevName(&pPAEWContext, szDeviceName, PAEW_DEV_TYPE_HID);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
		nDevCount = 1; //solo only
	}
	else
	{
		iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}

	for (i = 0; i < nDevCount; i++)
	{
		iRtn = PAEW_GetDevInfo(pPAEWContext, i, PAEW_DEV_INFOTYPE_LCD_STATE, &devInfo);
		if ((iRtn != PAEW_RET_SUCCESS) && (iRtn != PAEW_RET_NOT_SUPPORTED))
		{
			iRtn = -1;
			goto END;
		}
		iRtn = PAEW_GetDevInfo(pPAEWContext, i, PAEW_DEV_INFOTYPE_COS_TYPE, &devInfo);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}

		nDevInfoType = PAEW_DEV_INFOTYPE_COS_TYPE | PAEW_DEV_INFOTYPE_COS_VERSION | PAEW_DEV_INFOTYPE_SN | PAEW_DEV_INFOTYPE_CHAIN_TYPE | PAEW_DEV_INFOTYPE_PIN_STATE | PAEW_DEV_INFOTYPE_LIFECYCLE;
		if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_DRAGONBALL)
		{
			nDevInfoType |= (PAEW_DEV_INFOTYPE_N_T | PAEW_DEV_INFOTYPE_SESSKEY_HASH);
		}
		else if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_BIO)
		{
			nDevInfoType |= (PAEW_DEV_INFOTYPE_BLE_VERSION);
		}
		iRtn = PAEW_GetDevInfo(pPAEWContext, i, nDevInfoType, &devInfo);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
		iRtn = 0;
	}
END:
	PAEW_FreeContext(pPAEWContext);
	if (iRtn == 0)
		return create_devinfo(iRtn, devInfo);
	else
		return create_code(iRtn);
}

char *get_modifypin(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	size_t i = 0;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	for (i = 0; i < nDevCount; i++)
	{
		iRtn = PAEW_ChangePIN(pPAEWContext, i);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code(iRtn);
}
char *get_generate(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	PAEW_DevInfo devInfo;
	size_t i = 0;

	unsigned char pbMneWord[PAEW_MNE_MAX_LEN] = {0};
	size_t nMneWordLen = 0;
	char szCheckMneWord[PAEW_MNE_MAX_LEN + 1] = {0};
	size_t nCheckMneWordLen = 0;

	size_t pnCheckIndex[PAEW_MNE_INDEX_MAX_COUNT] = {0};
	size_t nCheckIndexCount = 0;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	iRtn = PAEW_GetDevInfo(pPAEWContext, 0, PAEW_DEV_INFOTYPE_COS_TYPE, &devInfo);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_PERSONAL)
	{
		for (i = 0; i < nDevCount; i++)
		{
			iRtn = PAEW_GenerateSeed(pPAEWContext, i, 16, 0, 0);
			if (iRtn != PAEW_RET_SUCCESS)
			{
				iRtn = -1;
				goto END;
			}
		}
	}
	else if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_DRAGONBALL)
	{
		iRtn = PAEW_GenerateSeed(pPAEWContext, 0, 0, (uint8_t)nDevCount, (uint8_t)(nDevCount - 1));
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}
	else if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_BIO)
	{
		iRtn = -1;
		goto END;
	}

	else
	{
		iRtn = -1;
		goto END;
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code(iRtn);
}
char *get_format(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	size_t i = 0;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	for (i = 0; i < nDevCount; i++)
	{
		iRtn = PAEW_Format(pPAEWContext, i);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code(iRtn);
}
char *get_import(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	PAEW_DevInfo devInfo;
	size_t i = 0;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	iRtn = PAEW_GetDevInfo(pPAEWContext, 0, PAEW_DEV_INFOTYPE_COS_TYPE, &devInfo);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_PERSONAL)
	{
		for (i = 0; i < nDevCount; i++)
		{
			iRtn = PAEW_ImportSeed(pPAEWContext, i, 0, 0);
			if (iRtn != PAEW_RET_SUCCESS)
			{
				iRtn = -1;
				goto END;
			}
		}
	}
	else if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_BIO)
	{
		iRtn = -1;
		goto END;
	}
	else if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_DRAGONBALL)
	{
		iRtn = PAEW_ImportSeed(pPAEWContext, 0, 0, 0);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}
	else
	{
		iRtn = -1;
		goto END;
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code(iRtn);
}
char *get_reboot(int port)
{
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	size_t i = 0;

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	for (i = 0; i < nDevCount; i++)
	{
		iRtn = PAEW_ClearCOS(pPAEWContext, i);
		if (iRtn != PAEW_RET_SUCCESS)
		{
			iRtn = -1;
			goto END;
		}
	}

	iRtn = 0;
END:
	PAEW_FreeContext(pPAEWContext);
	return create_code(iRtn);
}

char *post_address(int port, char *data)
{
	int coin = COIN_NOT_FOUND;
	int iRtn = -1;

	void *pPAEWContext = 0;
	size_t nDevCount;

	PAEW_DevInfo devInfo;
	size_t i = 0;

	unsigned char pbPublicKey[PAEW_PUBLIC_KEY_MAX_LEN] = {0};
	size_t nPublicKeyLen = 0;

	unsigned char pbAddressData[PAEW_COIN_ADDRESS_MAX_LEN] = {0};
	size_t nAddressDataLen = 0;

	unsigned int path[6] = {0};
	unsigned char strPath[130] = { 0 };

	int is_show = parse_showonscreen(data);
	printf("is_show %d\n", is_show);
	strPath[0] = 0x30;//0
	strPath[1] = 0x78;//x

	coin = parse_coin(data);
	fethc_path(coin, path);

	iRtn = PAEW_InitContext(&pPAEWContext, &nDevCount);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	iRtn = PAEW_GetDevInfo(pPAEWContext, 0, PAEW_DEV_INFOTYPE_COS_TYPE, &devInfo);
	if (iRtn != PAEW_RET_SUCCESS)
	{
		iRtn = -1;
		goto END;
	}

	if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_BIO)
	{
		iRtn = -1;
		goto END;
	}

	if (devInfo.ucCOSType == PAEW_DEV_INFO_COS_TYPE_PERSONAL)
	{
		for (i = 0; i < nDevCount; i++)
		{
			iRtn = PAEW_DeriveTradeAddress(pPAEWContext, i, coin, path, sizeof(path) / sizeof(path[0]));
			if (iRtn != PAEW_RET_SUCCESS)
			{
				iRtn = -1;
				goto END;
			}
			nAddressDataLen = sizeof(pbAddressData);
			iRtn = PAEW_GetTradeAddress(pPAEWContext, 0, coin, is_show, pbAddressData, &nAddressDataLen);
			if (iRtn != PAEW_RET_SUCCESS)
			{
				iRtn = -1;
				goto END;
			}
		}
	}
END:
	PAEW_FreeContext(pPAEWContext);
	if(iRtn==PAEW_RET_SUCCESS)
	{	
		switch (coin)
		{
		case PAEW_COIN_TYPE_ETH:
		case PAEW_COIN_TYPE_ETC:
			memcpy(strPath+2,pbAddressData,nAddressDataLen);
			break;
		default:
			memcpy(strPath,pbAddressData,nAddressDataLen);
			break;
		}
		return create_code_result_str(PAEW_RET_SUCCESS,"address",strPath);
	}
	else
	{
		return create_code(iRtn);
	}
}
char *post_sign(int port, char *data)
{
	int coin = COIN_NOT_FOUND;
	//unsigned char tx[] = { 0xec, 0x09, 0x85, 0x04, 0xa8, 0x17, 0xc8, 0x00, 0x82, 0x52, 0x08, 0x94, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x88, 0x0d, 0xe0, 0xb6, 0xb3, 0xa7, 0x64, 0x00, 0x00, 0x80, 0x01, 0x80, 0x80 };
	coin = parse_coin(data);
	return sign_tx(coin,data);
}
char *post_xrppub(int port, char *data)
{
	return create_code(0);
}
char *post_update(int port, char *data)
{
	return create_code(0);
}
char *post_verify(int port, char *data)
{
	return create_code(0);
}
