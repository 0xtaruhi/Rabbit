#include "pch.h"
#include "SMIMS_API.h"

#if SMIMS_BUILD == 0x1036

//=============================================================

USB_HANDLE SMIMS_DriverOpen(const CHAR *devname, INT devnum)
{
	WORD VID = 0x2200;
	WORD PID;

	if(strcmp(devname, "VLX2") == 0)
		PID = 0x2008;
	else if(strcmp(devname, "VEX2") == 0)
		PID = 0x2009;
	else if(strcmp(devname, "VLA2") == 0)
		PID = 0x200C;
	else if(strcmp(devname, "VEA2") == 0)
		PID = 0x200D;
	else
		PID = 0x0000;

	if(PID == 0x0000)
		return NULL;

	return usb_device_open(VID, PID, devnum);
}

BOOL SMIMS_DriverClose(USB_HANDLE dev_handle)
{
	return usb_device_close(dev_handle);
}

BOOL SMIMS_EngineReset(USB_HANDLE dev_handle)
{
	BYTE Command;

	Command = 0x02;
	if(!SMIMS_WriteUSB(dev_handle, EP4, &Command, sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

//============ Encrypt API ==============

BOOL SMIMS_EncryptTableRead(USB_HANDLE dev_handle, WORD *EncryptTBL)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x0f;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	if(!SMIMS_ReadUSB(dev_handle, EP6, EncryptTBL, 32*sizeof(WORD)))
		return FALSE;

	if(!SMIMS_CommandActive(dev_handle))
		return FALSE;

	return TRUE;
}

VOID SMIMS_EncryptTableDecode(WORD *EncryptTBL, UINT *encindex, UINT *decindex)
{
	UINT i;
	WORD temp;

	EncryptTBL[0] = ~EncryptTBL[0];
	temp = EncryptTBL[0];

	for(i = 1; i < 32; i++)
	{
		EncryptTBL[i] ^= temp;
		temp = EncryptTBL[i];
	}

	*encindex = 0;
	*decindex = 0;
}

VOID SMIMS_EncryptData(WORD *dataptr, UINT Length, WORD *EncryptTBL, UINT *encindex)
{
	UINT i = 0;
	UINT j = *encindex;

	while(i < Length)
	{
		dataptr[i++] ^= EncryptTBL[j++];
		j &= 0x0f;
	}

	*encindex = j;
}

VOID SMIMS_DecryptData(WORD *dataptr, UINT Length, WORD *EncryptTBL, UINT *decindex)
{
	UINT i = 0;
	UINT j = *decindex;
	WORD *encptr = EncryptTBL + 16;

	while(i < Length)
	{
		dataptr[i++] ^= encptr[j++];
		j &= 0x0f;
	}

	*decindex = j;
}

VOID SMIMS_EncryptCopy(WORD *dst, WORD *src, UINT Length, WORD *EncryptTBL, UINT *encindex)
{
	UINT i = 0;
	UINT j = *encindex;

	while(i < Length)
	{
		dst[i] = src[i] ^ EncryptTBL[j];
		i++;
		j++;
		j &= 0x0f;
	}

	*encindex = j;
}

VOID SMIMS_DecryptCopy(WORD *dst, WORD *src, UINT Length, WORD *EncryptTBL, UINT *decindex)
{
	UINT i = 0;
	UINT j = *decindex;
	WORD *encptr = EncryptTBL + 16;

	while(i < Length)
	{
		dst[i] = src[i] ^ encptr[j];
		i++;
		j++;
		j &= 0x0f;
	}

	*decindex = j;
}

WORD SMIMS_LicenseGen(WORD SecurityKey, WORD CustomerID)
{
	DWORD temp = 0;
	WORD i = SecurityKey & 0x0003;
	WORD j = (CustomerID & 0x000f) << 4;

	j >>= i;
	j = (j >> 4) | (j & 0x000f);
	temp |= (j << 16);

	i = (SecurityKey & 0x0030) >> 4;
	j = CustomerID & 0x00f0;
	j >>= i;
	j = (j >> 4) | (j & 0x000f);
	temp |= (j << 20);

	i = (SecurityKey & 0x0300) >> 8;
	j = (CustomerID & 0x0f00) >> 4;
	j >>= i;
	j = (j >> 4) | (j & 0x000f);
	temp |= (j << 24);

	i = (SecurityKey & 0x3000) >> 12;
	j = (CustomerID & 0xf000) >> 8;
	j >>= i;
	j = (j >> 4) | (j & 0x000f);
	temp |= (j << 28);

	temp >>= 11;
	i = (WORD) ~((temp >> 16) | (temp & 0x0000ffff));

	return i;
}

//============ Data Transfer API ==============

BOOL SMIMS_FIFO_Write(USB_HANDLE dev_handle, WORD *ptr, INT Length)
{
	if(!SMIMS_WriteUSB(dev_handle, EP2, ptr, Length*sizeof(WORD)))
		return FALSE;
	return TRUE;
}

BOOL SMIMS_FIFO_Read(USB_HANDLE dev_handle, WORD *ptr, INT Length)
{
	if(!SMIMS_ReadUSB(dev_handle, EP6, ptr, Length*sizeof(WORD)))
		return FALSE;
	return TRUE;
}

//============ Command API ==============

BOOL SMIMS_SyncDelay(USB_HANDLE dev_handle)
{
	BYTE Command;

	do{
		Command = 0x00;
		if(!SMIMS_WriteUSB(dev_handle, EP4, &Command, sizeof(BYTE)))
			return FALSE;

		if(!SMIMS_ReadUSB(dev_handle, EP8, &Command, sizeof(BYTE)))
			return FALSE;
	} while(!Command);

	return TRUE;
}

BOOL SMIMS_CommandActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if (!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x00;
	if (!SMIMS_WriteUSB(dev_handle, EP4, Command, 2 * sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_CFGSpaceRead(USB_HANDLE dev_handle, struct SMIMS_CFGSpace *pCFGSpace)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x01;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	if(!SMIMS_ReadUSB(dev_handle, EP6, pCFGSpace, sizeof(SMIMS_CFGSpace)))
		return FALSE;

	if(!SMIMS_CommandActive(dev_handle))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_CFGSpaceWrite(USB_HANDLE dev_handle, struct SMIMS_CFGSpace *pCFGSpace)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x11;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	if(!SMIMS_WriteUSB(dev_handle, EP2, pCFGSpace, sizeof(SMIMS_CFGSpace)))
		return FALSE;

	if(!SMIMS_CommandActive(dev_handle))
		return FALSE;

	return true;
}

BOOL SMIMS_FPGAProgrammerActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x02;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriCommActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x03;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriInstrumentActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x08;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriLinkActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x09;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriSoCActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x0a;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriCommProActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x0b;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_VeriSDKActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x04;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_FlashReadActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x05;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

BOOL SMIMS_FlashWriteActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return FALSE;

	Command[0] = 0x01;
	Command[1] = 0x15;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return FALSE;

	return TRUE;
}

//============= Configuration Space API ===============

WORD SMIMS_GetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[0];
}

WORD SMIMS_GetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[1];
}

BYTE SMIMS_GetVeriComm_ISV(struct SMIMS_CFGSpace *pCFGSpace)
{
	WORD temp = pCFGSpace->CFG[2] >> 4;
	temp &= 0x000f;
	return (BYTE) temp;
}

BOOL SMIMS_IsVeriComm_ClockCheck_Enable(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[2] & 0x0001)
		return TRUE;
	else
		return FALSE;
}

BYTE SMIMS_GetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[3] & 0xFF;
}

BYTE SMIMS_GetModeSelector(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[3] >> 8;
}

WORD SMIMS_GetFlashBeginBlockAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[4];
}

WORD SMIMS_GetFlashBeginClusterAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[5];
}

WORD SMIMS_GetFlashReadEndBlockAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[6];
}

WORD SMIMS_GetFlashReadEndClusterAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[7];
}

WORD SMIMS_GetSecurityKey(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[31];
}

//====================================================================

VOID SMIMS_SetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace *pCFGSpace, WORD ClockHighDelay)
{
	pCFGSpace->CFG[0] = ClockHighDelay;
}

VOID SMIMS_SetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace *pCFGSpace, WORD ClockLowDelay)
{
	pCFGSpace->CFG[1] = ClockLowDelay;
}

VOID SMIMS_SetVeriComm_ISV(struct SMIMS_CFGSpace *pCFGSpace, WORD Value)
{
	WORD temp1 = pCFGSpace->CFG[2] & 0x0001;
	WORD temp2 = (Value << 4) & 0x00f0;
	pCFGSpace->CFG[2] = temp1 | temp2;
}

VOID SMIMS_SetVeriComm_ClockCheck(struct SMIMS_CFGSpace *pCFGSpace, BOOL Check)
{
	if(Check)
		pCFGSpace->CFG[2] |= 0x0001;
	else
		pCFGSpace->CFG[2] &= 0xfffe;
}

VOID SMIMS_SetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace *pCFGSpace, BYTE Select)
{
	pCFGSpace->CFG[3] &= 0xff00;
	pCFGSpace->CFG[3] |= Select;
}

VOID SMIMS_SetModeSelector(struct SMIMS_CFGSpace *pCFGSpace, BYTE Select)
{
	pCFGSpace->CFG[3] &= 0x00ff;
	pCFGSpace->CFG[3] |= (Select << 8);
}

VOID SMIMS_SetFlashBeginBlockAddr(struct SMIMS_CFGSpace *pCFGSpace, WORD Address)
{
	pCFGSpace->CFG[4] = Address;
}

VOID SMIMS_SetFlashBeginClusterAddr(struct SMIMS_CFGSpace *pCFGSpace, WORD Address)
{
	pCFGSpace->CFG[5] = Address;
}

VOID SMIMS_SetFlashReadEndBlockAddr(struct SMIMS_CFGSpace *pCFGSpace, WORD Address)
{
	pCFGSpace->CFG[6] = Address;
}

VOID SMIMS_SetFlashReadEndClusterAddr(struct SMIMS_CFGSpace *pCFGSpace, WORD Address)
{
	pCFGSpace->CFG[7] = Address;
}

VOID SMIMS_SetLicenseKey(struct SMIMS_CFGSpace *pCFGSpace, WORD LicenseKey)
{
	pCFGSpace->CFG[31] = LicenseKey;
}

//====================================================================

INT SMIMS_Version(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[32];
}

INT SMIMS_MajorVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return (pCFGSpace->CFG[32] >> 8);
}

INT SMIMS_SubVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return (pCFGSpace->CFG[32] >> 4) & 0xF;
}

INT SMIMS_SubSubVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[32] & 0xF;
}

WORD SMIMS_GetFIFOSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[33];
}

WORD SMIMS_GetFlashTotalBlock(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[34];
}

WORD SMIMS_GetFlashBlockSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[35];
}

WORD SMIMS_GetFlashClusterSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[36];
}

BOOL SMIMS_VeriCommAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0001)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_VeriInstrumentAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0002)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_VeriLinkAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0004)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_VeriSoCAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0008)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_VeriCommProAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0010)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_VeriSDKAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0100)
		return TRUE;
	else
		return FALSE;
}

//====================================================================

BOOL SMIMS_IsFPGAProgram(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[48] & 0x0001)
		return TRUE;
	else
		return FALSE;
}

BOOL SMIMS_IsPCBConnect(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[48] & 0x0100)
		return FALSE;
	else
		return TRUE;
}

BOOL SMIMS_IsVeriComm_ClockContinue(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[49] & 0x0001)
		return FALSE;

	return TRUE;
}

//====================================================================
#endif
