
#include "SMIMS_iobase.h"

#ifndef H_SMIMS_API
#define H_SMIMS_API

#define SMIMS_BUILD 0x1036

//========================================

struct SMIMS_CFGSpace
{
	WORD CFG[64];
};

//========================================
USB_HANDLE SMIMS_DriverOpen(const CHAR* devname, INT devnum);
BOOL SMIMS_DriverClose(USB_HANDLE dev_handle);

BOOL SMIMS_EngineReset(USB_HANDLE dev_handle);

//============ Encrypt API ==============
BOOL SMIMS_EncryptTableRead(USB_HANDLE dev_handle, WORD* EncryptTBL);
VOID SMIMS_EncryptTableDecode(WORD* EncryptTBL, UINT* encindex, UINT* decindex);
VOID SMIMS_EncryptData(WORD* dataptr, UINT Length, WORD* EncryptTBL, UINT* encindex);
VOID SMIMS_DecryptData(WORD* dataptr, UINT Length, WORD* EncryptTBL, UINT* decindex);
VOID SMIMS_EncryptCopy(WORD* dst, WORD* src, UINT Length, WORD* EncryptTBL, UINT* encindex);
VOID SMIMS_DecryptCopy(WORD* dst, WORD* src, UINT Length, WORD* EncryptTBL, UINT* decindex);

WORD SMIMS_LicenseGen(WORD SecurityKey, WORD CustomerID);

//============ Data Transfer API ==============
BOOL SMIMS_FIFO_Write(USB_HANDLE dev_handle, WORD* ptr, INT Length);
BOOL SMIMS_FIFO_Read(USB_HANDLE dev_handle, WORD* ptr, INT Length);

//============ Command API ==============
BOOL SMIMS_SyncDelay(USB_HANDLE dev_handle);
BOOL SMIMS_CommandActive(USB_HANDLE dev_handle);

BOOL SMIMS_CFGSpaceRead(USB_HANDLE dev_handle, struct SMIMS_CFGSpace* pSMIMS_CFGSpace);
BOOL SMIMS_CFGSpaceWrite(USB_HANDLE dev_handle, struct SMIMS_CFGSpace* pSMIMS_CFGSpace);

BOOL SMIMS_FPGAProgrammerActive(USB_HANDLE dev_handle);

BOOL SMIMS_VeriCommActive(USB_HANDLE dev_handle);
BOOL SMIMS_VeriInstrumentActive(USB_HANDLE dev_handle);
BOOL SMIMS_VeriLinkActive(USB_HANDLE dev_handle);
BOOL SMIMS_VeriSoCActive(USB_HANDLE dev_handle);
BOOL SMIMS_VeriCommProActive(USB_HANDLE dev_handle);
BOOL SMIMS_VeriSDKActive(USB_HANDLE dev_handle);

BOOL SMIMS_FlashReadActive(USB_HANDLE dev_handle);  // Flash API
BOOL SMIMS_FlashWriteActive(USB_HANDLE dev_handle);  // Flash API

//============ Configuration Space API ==============
WORD SMIMS_GetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace* pCFGSpace);
WORD SMIMS_GetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace* pCFGSpace);
BYTE SMIMS_GetVeriComm_ISV(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_IsVeriComm_ClockCheck_Enable(struct SMIMS_CFGSpace* pCFGSpace);
BYTE SMIMS_GetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace* pCFGSpace);
BYTE SMIMS_GetModeSelector(struct SMIMS_CFGSpace* pCFGSpace);
WORD SMIMS_GetFlashBeginBlockAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetFlashBeginClusterAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetFlashReadEndBlockAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetFlashReadEndClusterAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetSecurityKey(struct SMIMS_CFGSpace* pCFGSpace);

VOID SMIMS_SetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace* pCFGSpace, WORD ClockHighDelay);
VOID SMIMS_SetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace* pCFGSpace, WORD ClockLowDelay);
VOID SMIMS_SetVeriComm_ISV(struct SMIMS_CFGSpace* pCFGSpace, WORD Value);
VOID SMIMS_SetVeriComm_ClockCheck(struct SMIMS_CFGSpace* pCFGSpace, BOOL Check);
VOID SMIMS_SetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace* pCFGSpace, BYTE Select);
VOID SMIMS_SetModeSelector(struct SMIMS_CFGSpace* pCFGSpace, BYTE Select);
VOID SMIMS_SetFlashBeginBlockAddr(struct SMIMS_CFGSpace* pCFGSpace, WORD Address);  // Flash API
VOID SMIMS_SetFlashBeginClusterAddr(struct SMIMS_CFGSpace* pCFGSpace, WORD Address);  // Flash API
VOID SMIMS_SetFlashReadEndBlockAddr(struct SMIMS_CFGSpace* pCFGSpace, WORD Address);  // Flash API
VOID SMIMS_SetFlashReadEndClusterAddr(struct SMIMS_CFGSpace* pCFGSpace, WORD Address);  // Flash API
VOID SMIMS_SetLicenseKey(struct SMIMS_CFGSpace* pCFGSpace, WORD LicenseKey);

INT SMIMS_Version(struct SMIMS_CFGSpace* pCFGSpace);
INT SMIMS_MajorVersion(struct SMIMS_CFGSpace* pCFGSpace);
INT SMIMS_SubVersion(struct SMIMS_CFGSpace* pCFGSpace);
INT SMIMS_SubSubVersion(struct SMIMS_CFGSpace* pCFGSpace);
WORD SMIMS_GetFIFOSize(struct SMIMS_CFGSpace* pCFGSpace);
WORD SMIMS_GetFlashTotalBlock(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetFlashBlockSize(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
WORD SMIMS_GetFlashClusterSize(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
BOOL SMIMS_VeriCommAbility(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_VeriInstrumentAbility(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_VeriLinkAbility(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_VeriSoCAbility(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_VeriCommProAbility(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_VeriSDKAbility(struct SMIMS_CFGSpace* pCFGSpace);

BOOL SMIMS_IsFPGAProgram(struct SMIMS_CFGSpace* pCFGSpace);
BOOL SMIMS_IsPCBConnect(struct SMIMS_CFGSpace* pCFGSpace);  // Multiple PCB API
BOOL SMIMS_IsVeriComm_ClockContinue(struct SMIMS_CFGSpace* pCFGSpace);

//==================================================

#endif
