/**
 *
 * \file
 *
 * \brief BSD compatible socket interface.
 *
 * Copyright (c) 2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifdef __ATECC508__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "driver/include/ecc_types.h"
#include "driver/source/m2m_hif.h"
#include "ecc_crypto_ecc508.h"
#include "socket/include/socket.h"
#include "cryptoauthlib/lib/cryptoauthlib.h"
#include "cryptoauthlib/lib/tls/atcatls.h"
#include "cryptoauthlib/lib/basic/atca_basic.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define __DBG__

#ifdef __DBG__
#define ECC_DBG								M2M_INFO
#define M2M_DUMP_BUF(name,Buffer,size)					\
do														\
{														\
	int k;												\
	uint8	*buf = Buffer;								\
	printf("%s(%u)",name, size);	\
	for (k = 0; k < size; k++)							\
	{													\
		if (!(k % 8))									\
		printf("\r\n\t");								\
		printf("0x%02X, ", buf[k]);						\
	}													\
	printf("\r\n");									\
}while(0)
#else
#define M2M_DUMP_BUF(name,Buffer,size)
#define ECC_DBG(...)
#endif


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
GLOBALS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

volatile uint16		gu16SlotIdx = 2;



/************************************************************/
static sint8 ecdhDeriveClientSharedSecret
(
tstrECPoint			*pstrServerPubKey, 
uint8				*pu8ECDHSharedSecret, 
tstrECPoint			*pstrClientPubKey
)
{
	uint16	u16KeySlot	= gu16SlotIdx;
	sint8	s8Ret		= M2M_ERR_FAIL;
	ATCA_STATUS rs;
	
	ECC_DBG("ECDH: create_key: keySlot: %d\n", u16KeySlot);
	if(atcatls_create_key(u16KeySlot, pstrClientPubKey->X) == ATCA_SUCCESS)
	{
		pstrClientPubKey->u16Size = 32;
		ECC_DBG("ECDH: ecdh: keySlot: %d\n", u16KeySlot);
		if((rs=atcab_ecdh(u16KeySlot, pstrServerPubKey->X, pu8ECDHSharedSecret)) == ATCA_SUCCESS)
		{
			s8Ret = M2M_SUCCESS;
			gu16SlotIdx ++;
			if(gu16SlotIdx == 5)
				gu16SlotIdx = 1;
		}
		ECC_DBG("ECDH: Status: %d\n", rs);
	}
	return s8Ret;
}

/************************************************************/
static sint8 ecdhDeriveKeyPair
(
tstrECPoint		*pstrSrvPubKey
)
{
	uint16	u16KeySlot		= gu16SlotIdx;
	sint8	ret				= M2M_ERR_FAIL;
	
	ECC_DBG("DRV_SLOT = %u\n", u16KeySlot);
	if(atcatls_create_key(u16KeySlot, pstrSrvPubKey->X) == ATCA_SUCCESS)
	{
		pstrSrvPubKey->u16Size		= 32;
		pstrSrvPubKey->u16PrivKeyID = u16KeySlot;

		gu16SlotIdx ++;
		if(gu16SlotIdx == 5)
		gu16SlotIdx = 1;

		ret = M2M_SUCCESS;
	}

	return ret;
}

/************************************************************/
static sint8 ecdhDeriveServerSharedSecret
(
uint16			u16PrivKeyID,
tstrECPoint		*pstrClientPubKey,
uint8			*pu8ECDHSharedSecret
)
{
	uint16	u16KeySlot	= u16PrivKeyID;
	sint8	s8Ret		= M2M_ERR_FAIL;
	uint8	u8AtcaRet;

	ECC_DBG("__SLOT = %u\n", u16KeySlot);
	u8AtcaRet = atcab_ecdh(u16KeySlot, pstrClientPubKey->X, pu8ECDHSharedSecret);
	if(u8AtcaRet == ATCA_SUCCESS)
	{
		s8Ret = M2M_SUCCESS;
	}
	else
	{
		M2M_INFO("__SLOT = %u, Err = %X\n", u16KeySlot, u8AtcaRet);
	}
	return s8Ret;
}

/************************************************************/
static sint8 ecdsaProcessSignVerifREQ(uint32 u32NumSig, uint32 u32ReadAddr)
{
	sint8		ret			= M2M_ERR_FAIL;
	tstrECPoint	Key;
	uint32		u32Idx;
	uint8		au8Sig[80];
	uint8		au8Hash[80] = {0};
	uint8		bSetRxDone	= 1;
	uint16		u16CurveType, u16HashSz, u16SigSz, u16KeySz;
	
	for(u32Idx = 0; u32Idx < u32NumSig; u32Idx ++)
	{
		if(hif_receive(u32ReadAddr, (uint8*)&u16CurveType, 2, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += 2;

		if(hif_receive(u32ReadAddr, (uint8*)&u16KeySz, 2, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += 2;

		if(hif_receive(u32ReadAddr, (uint8*)&u16HashSz, 2, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += 2;

		if(hif_receive(u32ReadAddr, (uint8*)&u16SigSz, 2, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += 2;

		u16CurveType	= _htons(u16CurveType);
		Key.u16Size		= _htons(u16KeySz);
		u16HashSz		= _htons(u16HashSz);
		u16SigSz		= _htons(u16SigSz);
		
		if(hif_receive(u32ReadAddr, Key.X, Key.u16Size * 2, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += (Key.u16Size * 2);

		if(hif_receive(u32ReadAddr, au8Hash, u16HashSz, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += u16HashSz;

		if(hif_receive(u32ReadAddr, au8Sig, u16SigSz, 0) != M2M_SUCCESS) goto __ERR;
		u32ReadAddr += u16SigSz;
		
		bSetRxDone = 0;
		if(u16CurveType == EC_SECP256R1)
		{
			bool	bIsVerified = false;
			
			ret = atcatls_verify(au8Hash, au8Sig, Key.X, &bIsVerified);
			if(ret == ATCA_SUCCESS)
			{
				ret = (bIsVerified == true) ? M2M_SUCCESS : M2M_ERR_FAIL;
				if(bIsVerified == false)
				{
					M2M_INFO("ECDSA SigVerif FAILED<%d>\n", u16HashSz);
				}
			}
			else
			{
				ECC_DBG("atcatls_verify (%X)\n", ret);
			}
			
			if(ret != M2M_SUCCESS)
			{
				break;
			}
		}
		else
		{
				
		}
	}
	
__ERR:
	if(bSetRxDone)
		hif_receive(0, NULL, 0, 1);
	
	return ret;
}

/************************************************************/
static sint8 ecdsaProcessSignGenREQ(uint32 u32ReadAddr, tstrEcdsaSignReqInfo *pstrSignREQ, uint8 *pu8Sig, uint16 *pu16SigSz)
{
	sint8		ret		= M2M_ERR_FAIL;
	uint8		au8Hash[32];
	uint8		bSetRxDone	= 1;
			
	if(hif_receive(u32ReadAddr, au8Hash, pstrSignREQ->u16HashSz, 0) != M2M_SUCCESS) goto __ERR;
	bSetRxDone = 0;
	if(pstrSignREQ->u16CurveType == EC_SECP256R1)
	{
		*pu16SigSz = 64;
		ret = atcatls_sign(0, au8Hash, pu8Sig);
		{
			uint8	key[64];
			atcatls_calc_pubkey(0, key);
			M2M_DUMP_BUF("KEY", key, 64);
		}
		M2M_DUMP_BUF("H", au8Hash, 32);
		M2M_DUMP_BUF("SIG", pu8Sig, 64);
	}
	else
	{
			
	}

__ERR:
	if(bSetRxDone)
		hif_receive(0, NULL, 0, 1);

	return ret;
}

/************************************************************/
static void eccProcessREQ(uint8 u8OpCode, uint16 u16DataSize, uint32 u32Addr)
{
	tstrEccReqInfo	strEccREQ;
	tstrEccReqInfo	strECCResp;
	uint8			au8Sig[80];
	uint16			u16RspDataSz	= 0;
	uint8			*pu8RspDataBuff	= NULL;
	
	strECCResp.u16Status	= 1;

	ECC_DBG("PREQ: opcode: %d, size: %d, addr: %d\n", u8OpCode, u16DataSize, u32Addr);
	if(hif_receive(u32Addr, (uint8*)&strEccREQ, sizeof(tstrEccReqInfo), 0) == M2M_SUCCESS)
	{
		ECC_DBG("PREQ: hif_receive: req: %d, status: %d, seq: %d, ud: %d\n", strEccREQ.u16REQ, strEccREQ.u16Status, strEccREQ.u32SeqNo, strEccREQ.u32UserData);
		switch(strEccREQ.u16REQ)
		{
		case ECC_REQ_CLIENT_ECDH: // 1
			strECCResp.u16Status = ecdhDeriveClientSharedSecret(&strEccREQ.strEcdhREQ.strPubKey, strECCResp.strEcdhREQ.au8Key, &strECCResp.strEcdhREQ.strPubKey);
			M2M_DUMP_BUF("strPubKey", &strECCResp.strEcdhREQ.strPubKey, sizeof(strECCResp.strEcdhREQ.strPubKey));
			break;

		case ECC_REQ_GEN_KEY: // 3
			strECCResp.u16Status = ecdhDeriveKeyPair(&strECCResp.strEcdhREQ.strPubKey);
			break;

		case ECC_REQ_SERVER_ECDH: // 2
			strECCResp.u16Status = ecdhDeriveServerSharedSecret(strEccREQ.strEcdhREQ.strPubKey.u16PrivKeyID, &strEccREQ.strEcdhREQ.strPubKey, strECCResp.strEcdhREQ.au8Key);
			break;
			
		case ECC_REQ_SIGN_VERIFY: // 5
			strECCResp.u16Status = ecdsaProcessSignVerifREQ(strEccREQ.strEcdsaVerifyREQ.u32nSig, (u32Addr + sizeof(tstrEccReqInfo)));
			break;
			
		case ECC_REQ_SIGN_GEN: // 4
			strECCResp.u16Status = ecdsaProcessSignGenREQ((u32Addr + sizeof(tstrEccReqInfo)), &strEccREQ.strEcdsaSignREQ, au8Sig, &u16RspDataSz);
			pu8RspDataBuff = au8Sig;
			break;
		}
		ECC_DBG("PREQ: Status: %d\n", strECCResp.u16Status);

		strECCResp.u16REQ		= strEccREQ.u16REQ;
		strECCResp.u32UserData	= strEccREQ.u32UserData;
		strECCResp.u32SeqNo		= strEccREQ.u32SeqNo;
	}
	hif_send(M2M_REQ_GROUP_SSL, (M2M_SSL_RESP_ECC | M2M_REQ_DATA_PKT), (uint8*)&strECCResp, sizeof(tstrEccReqInfo), pu8RspDataBuff, u16RspDataSz, sizeof(tstrEccReqInfo));
}

/************************************************************/
void eccInit(tenuEcc508TargetOp enuTargetMode)
{
	uint8	ret;
	bool	lock_state = false;
	
	hif_register_cb(M2M_REQ_GROUP_SSL, eccProcessREQ);
	ret = atcab_init(&cfg_ateccx08a_i2c_default);
	if(ret == ATCA_SUCCESS)
	{
		M2M_INFO("ATECC508 INIT OK\n");
	}
	else
	{
		M2M_INFO("ATECC508 INIT FAIL <%0X>\n", ret);		
	}
#if 1
	{
		uint8	configZone[128];
		ret = atcab_read_ecc_config_zone(configZone);
		M2M_INFO("ATECC508 Read %X\r\n", ret);
		M2M_DUMP_BUF("configZone", configZone, sizeof(configZone));
	}
#endif
	ret = atcab_is_locked(ATCA_ZONE_CONFIG, &lock_state);
	if(ret == ATCA_SUCCESS && lock_state == false)
	{
		uint8	configZone[128];
		uint8	u8KeyConfig00Addr	= 0x60;
		uint8	u8SlotConfig00Addr	= 0x14;
		uint8	lock_response		= 0x00;

		uint8	wincSlotConf[] = {
			0x8F, 0x20, //SlotConfig00
			0xC4, 0x44,
			0x87, 0x20,
			0x87, 0x20,
			0x8F, 0x0F,
		    0xC4, 0x36,
			0x9F, 0x0F,
			0x82, 0x20,
			0x0F, 0x0F,
			0xC4, 0x44,
			0x0F, 0x0F,
			0x0F, 0x0F,
			0x0F, 0x0F,
			0x0F, 0x0F,
			0x0F, 0x0F,
			0x0F, 0x0F /**/
		};

		uint8	wincKeyConf[] = {
			0x33, 0x00, //KeyConfig00
			0x1C, 0x00,
			0x13, 0x00,
			0x13, 0x00,
			0x7C, 0x00,
			0x1C, 0x00,
			0x3C, 0x00,
			0x33, 0x00,
			0x3C, 0x00,
			0x3C, 0x00,
			0x3C, 0x00,
			0x30, 0x00,
			0x3C, 0x00,
			0x3C, 0x00,
			0x3C, 0x00,
			0x30, 0x00/**/
		};

		/* 
			Read the current data of the config Zone.
		*/
		ret = atcab_read_ecc_config_zone(configZone);
		M2M_INFO("ATECC508 Read %X\r\n", ret);

		/*
			Change the configurations of Slot0:Slot4 with the WINC1500 selected settings.
		*/
		memcpy(&configZone[u8SlotConfig00Addr], wincSlotConf, sizeof(wincSlotConf));
		memcpy(&configZone[u8KeyConfig00Addr], wincKeyConf, sizeof(wincKeyConf));
		ret = atcab_write_ecc_config_zone(configZone);
		ECC_DBG("ATECC508 Write %X\r\n", ret);

		/*
			Lock Configuration And Data Zones
		*/
		ret = atcab_lock_config_zone(&lock_response);
		ECC_DBG("ATECC508 Lock Config %X\r\n", ret);
	
		ret = atcab_lock_data_zone(&lock_response);
		M2M_INFO("ATECC508 Lock Data %X\r\n", ret);
	}
	
	if((ret == 0) && (enuTargetMode == ECC508_TARGET_OP_TLS))
	{
		sslSetActiveCipherSuites(SSL_ECC_ALL_CIPHERS | SSL_NON_ECC_CIPHERS_AES_128);
	}
}


#if 0
/************************************************************/
static void eccSigTest(void)
{
	uint8		key[64] = {0}, Sig[64] = {0};
	uint8		msg[32];
	uint8		ret;
	bool		ver;
	
	atcatls_random(msg);
	
	ret = atcatls_calc_pubkey(0, key);
	if(ret == ATCA_SUCCESS)
	{
		printf("Key Gen\r\n");
		ret = atcatls_sign(0, msg, Sig);
		if(ret == ATCA_SUCCESS)
		{
			ret = atcatls_verify(msg, Sig, key, &ver);
			if(ret == ATCA_SUCCESS)
			{
				printf("Verification Status = %d\r\n", ver);
			}
			else
			{
				printf("VERIF ERROR %d\r\n", ret);
			}
		}
		else
		{
			printf("SIGN ERROR <%d>\r\n", ret);
		}
	}
	else
	{
		printf("Key Gen ERROR = %d\r\n", ret);
	}
}
#endif

#endif /* __WINC_ATECC508__ */