#include "pch.h"
#include "SMIMS_API.h"

#if SMIMS_BUILD == 0x1036

//=============================================================

USB_HANDLE SMIMS_DriverOpen(const char *devname, int devnum)
{
	uint16_t VID = 0x2200;
	uint16_t PID;

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

bool SMIMS_DriverClose(USB_HANDLE dev_handle)
{
	return usb_device_close(dev_handle);
}

bool SMIMS_EngineReset(USB_HANDLE dev_handle)
{
	BYTE Command;

	Command = 0x02;
	if(!SMIMS_WriteUSB(dev_handle, EP4, &Command, sizeof(BYTE)))
		return false;

	return true;
}

//============ Encrypt API ==============

bool SMIMS_EncryptTableRead(USB_HANDLE dev_handle, uint16_t *EncryptTBL)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x0f;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	if(!SMIMS_ReadUSB(dev_handle, EP6, EncryptTBL, 32*sizeof(uint16_t)))
		return false;

	if(!SMIMS_CommandActive(dev_handle))
		return false;

	return true;
}

void SMIMS_EncryptTableDecode(uint16_t *EncryptTBL, unsigned int *encindex, unsigned int *decindex)
{
	unsigned int i;
	uint16_t temp;

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

void SMIMS_EncryptData(uint16_t *dataptr, unsigned int Length, uint16_t *EncryptTBL, unsigned int *encindex)
{
	unsigned int i = 0;
	unsigned int j = *encindex;

	while(i < Length)
	{
		dataptr[i++] ^= EncryptTBL[j++];
		j &= 0x0f;
	}

	*encindex = j;
}

void SMIMS_DecryptData(uint16_t *dataptr, unsigned int Length, uint16_t *EncryptTBL, unsigned int *decindex)
{
	unsigned int i = 0;
	unsigned int j = *decindex;
	uint16_t *encptr = EncryptTBL + 16;

	while(i < Length)
	{
		dataptr[i++] ^= encptr[j++];
		j &= 0x0f;
	}

	*decindex = j;
}

void SMIMS_EncryptCopy(uint16_t *dst, uint16_t *src, unsigned int Length, uint16_t *EncryptTBL, unsigned int *encindex)
{
	unsigned int i = 0;
	unsigned int j = *encindex;

	while(i < Length)
	{
		dst[i] = src[i] ^ EncryptTBL[j];
		i++;
		j++;
		j &= 0x0f;
	}

	*encindex = j;
}

void SMIMS_DecryptCopy(uint16_t *dst, uint16_t *src, unsigned int Length, uint16_t *EncryptTBL, unsigned int *decindex)
{
	unsigned int i = 0;
	unsigned int j = *decindex;
	uint16_t *encptr = EncryptTBL + 16;

	while(i < Length)
	{
		dst[i] = src[i] ^ encptr[j];
		i++;
		j++;
		j &= 0x0f;
	}

	*decindex = j;
}

uint16_t SMIMS_LicenseGen(uint16_t SecurityKey, uint16_t CustomerID)
{
	DWORD temp = 0;
	uint16_t i = SecurityKey & 0x0003;
	uint16_t j = (CustomerID & 0x000f) << 4;

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
	i = (uint16_t) ~((temp >> 16) | (temp & 0x0000ffff));

	return i;
}

//============ Data Transfer API ==============

bool SMIMS_FIFO_Write(USB_HANDLE dev_handle, uint16_t *ptr, int Length)
{
	if(!SMIMS_WriteUSB(dev_handle, EP2, ptr, Length*sizeof(uint16_t)))
		return false;
	return true;
}

bool SMIMS_FIFO_Read(USB_HANDLE dev_handle, uint16_t *ptr, int Length)
{
	if(!SMIMS_ReadUSB(dev_handle, EP6, ptr, Length*sizeof(uint16_t)))
		return false;
	return true;
}

//============ Command API ==============

bool SMIMS_SyncDelay(USB_HANDLE dev_handle)
{
	BYTE Command;

	do{
		Command = 0x00;
		if(!SMIMS_WriteUSB(dev_handle, EP4, &Command, sizeof(BYTE)))
			return false;

		if(!SMIMS_ReadUSB(dev_handle, EP8, &Command, sizeof(BYTE)))
			return false;
	} while(!Command);

	return true;
}

bool SMIMS_CommandActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if (!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x00;
	if (!SMIMS_WriteUSB(dev_handle, EP4, Command, 2 * sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_CFGSpaceRead(USB_HANDLE dev_handle, struct SMIMS_CFGSpace *pCFGSpace)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x01;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	if(!SMIMS_ReadUSB(dev_handle, EP6, pCFGSpace, sizeof(SMIMS_CFGSpace)))
		return false;

	if(!SMIMS_CommandActive(dev_handle))
		return false;

	return true;
}

bool SMIMS_CFGSpaceWrite(USB_HANDLE dev_handle, struct SMIMS_CFGSpace *pCFGSpace)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x11;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	if(!SMIMS_WriteUSB(dev_handle, EP2, pCFGSpace, sizeof(SMIMS_CFGSpace)))
		return false;

	if(!SMIMS_CommandActive(dev_handle))
		return false;

	return true;
}

bool SMIMS_FPGAProgrammerActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x02;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriCommActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x03;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriInstrumentActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x08;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriLinkActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x09;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriSoCActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x0a;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriCommProActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x0b;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_VeriSDKActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x04;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_FlashReadActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x05;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

bool SMIMS_FlashWriteActive(USB_HANDLE dev_handle)
{
	BYTE Command[2];

	if(!SMIMS_SyncDelay(dev_handle))
		return false;

	Command[0] = 0x01;
	Command[1] = 0x15;
	if(!SMIMS_WriteUSB(dev_handle, EP4, Command, 2*sizeof(BYTE)))
		return false;

	return true;
}

//============= Configuration Space API ===============

uint16_t SMIMS_GetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[0];
}

uint16_t SMIMS_GetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[1];
}

BYTE SMIMS_GetVeriComm_ISV(struct SMIMS_CFGSpace *pCFGSpace)
{
	uint16_t temp = pCFGSpace->CFG[2] >> 4;
	temp &= 0x000f;
	return (BYTE) temp;
}

bool SMIMS_IsVeriComm_ClockCheck_Enable(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[2] & 0x0001)
		return true;
	else
		return false;
}

BYTE SMIMS_GetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[3] & 0xFF;
}

BYTE SMIMS_GetModeSelector(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[3] >> 8;
}

uint16_t SMIMS_GetFlashBeginBlockAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[4];
}

uint16_t SMIMS_GetFlashBeginClusterAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[5];
}

uint16_t SMIMS_GetFlashReadEndBlockAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[6];
}

uint16_t SMIMS_GetFlashReadEndClusterAddr(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[7];
}

uint16_t SMIMS_GetSecurityKey(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[31];
}

//====================================================================

void SMIMS_SetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace *pCFGSpace, uint16_t ClockHighDelay)
{
	pCFGSpace->CFG[0] = ClockHighDelay;
}

void SMIMS_SetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace *pCFGSpace, uint16_t ClockLowDelay)
{
	pCFGSpace->CFG[1] = ClockLowDelay;
}

void SMIMS_SetVeriComm_ISV(struct SMIMS_CFGSpace *pCFGSpace, uint16_t Value)
{
	uint16_t temp1 = pCFGSpace->CFG[2] & 0x0001;
	uint16_t temp2 = (Value << 4) & 0x00f0;
	pCFGSpace->CFG[2] = temp1 | temp2;
}

void SMIMS_SetVeriComm_ClockCheck(struct SMIMS_CFGSpace *pCFGSpace, bool Check)
{
	if(Check)
		pCFGSpace->CFG[2] |= 0x0001;
	else
		pCFGSpace->CFG[2] &= 0xfffe;
}

void SMIMS_SetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace *pCFGSpace, BYTE Select)
{
	pCFGSpace->CFG[3] &= 0xff00;
	pCFGSpace->CFG[3] |= Select;
}

void SMIMS_SetModeSelector(struct SMIMS_CFGSpace *pCFGSpace, BYTE Select)
{
	pCFGSpace->CFG[3] &= 0x00ff;
	pCFGSpace->CFG[3] |= (Select << 8);
}

void SMIMS_SetFlashBeginBlockAddr(struct SMIMS_CFGSpace *pCFGSpace, uint16_t Address)
{
	pCFGSpace->CFG[4] = Address;
}

void SMIMS_SetFlashBeginClusterAddr(struct SMIMS_CFGSpace *pCFGSpace, uint16_t Address)
{
	pCFGSpace->CFG[5] = Address;
}

void SMIMS_SetFlashReadEndBlockAddr(struct SMIMS_CFGSpace *pCFGSpace, uint16_t Address)
{
	pCFGSpace->CFG[6] = Address;
}

void SMIMS_SetFlashReadEndClusterAddr(struct SMIMS_CFGSpace *pCFGSpace, uint16_t Address)
{
	pCFGSpace->CFG[7] = Address;
}

void SMIMS_SetLicenseKey(struct SMIMS_CFGSpace *pCFGSpace, uint16_t LicenseKey)
{
	pCFGSpace->CFG[31] = LicenseKey;
}

//====================================================================

int SMIMS_Version(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[32];
}

int SMIMS_MajorVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return (pCFGSpace->CFG[32] >> 8);
}

int SMIMS_SubVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return (pCFGSpace->CFG[32] >> 4) & 0xF;
}

int SMIMS_SubSubVersion(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[32] & 0xF;
}

uint16_t SMIMS_GetFIFOSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[33];
}

uint16_t SMIMS_GetFlashTotalBlock(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[34];
}

uint16_t SMIMS_GetFlashBlockSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[35];
}

uint16_t SMIMS_GetFlashClusterSize(struct SMIMS_CFGSpace *pCFGSpace)
{
	return pCFGSpace->CFG[36];
}

bool SMIMS_VeriCommAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0001)
		return true;
	else
		return false;
}

bool SMIMS_VeriInstrumentAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0002)
		return true;
	else
		return false;
}

bool SMIMS_VeriLinkAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0004)
		return true;
	else
		return false;
}

bool SMIMS_VeriSoCAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0008)
		return true;
	else
		return false;
}

bool SMIMS_VeriCommProAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0010)
		return true;
	else
		return false;
}

bool SMIMS_VeriSDKAbility(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[37] & 0x0100)
		return true;
	else
		return false;
}

//====================================================================

bool SMIMS_IsFPGAProgram(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[48] & 0x0001)
		return true;
	else
		return false;
}

bool SMIMS_IsPCBConnect(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[48] & 0x0100)
		return false;
	else
		return true;
}

bool SMIMS_IsVeriComm_ClockContinue(struct SMIMS_CFGSpace *pCFGSpace)
{
	if(pCFGSpace->CFG[49] & 0x0001)
		return false;

	return true;
}

//====================================================================
#endif
