
#include "SMIMS_iobase.h"

#ifndef H_SMIMS_API
#define H_SMIMS_API

#define SMIMS_BUILD 0x1036

//========================================

struct SMIMS_CFGSpace
{
	uint16_t CFG[64];
};

//========================================
USB_HANDLE SMIMS_DriverOpen(const char* devname, int devnum);
bool SMIMS_DriverClose(USB_HANDLE dev_handle);

bool SMIMS_EngineReset(USB_HANDLE dev_handle);

//============ Encrypt API ==============
bool SMIMS_EncryptTableRead(USB_HANDLE dev_handle, uint16_t* EncryptTBL);
void SMIMS_EncryptTableDecode(uint16_t* EncryptTBL, unsigned int* encindex, unsigned int* decindex);
void SMIMS_EncryptData(uint16_t* dataptr, unsigned int Length, uint16_t* EncryptTBL, unsigned int* encindex);
void SMIMS_DecryptData(uint16_t* dataptr, unsigned int Length, uint16_t* EncryptTBL, unsigned int* decindex);
void SMIMS_EncryptCopy(uint16_t* dst, uint16_t* src, unsigned int Length, uint16_t* EncryptTBL, unsigned int* encindex);
void SMIMS_DecryptCopy(uint16_t* dst, uint16_t* src, unsigned int Length, uint16_t* EncryptTBL, unsigned int* decindex);

uint16_t SMIMS_LicenseGen(uint16_t SecurityKey, uint16_t CustomerID);

//============ Data Transfer API ==============
bool SMIMS_FIFO_Write(USB_HANDLE dev_handle, uint16_t* ptr, int Length);
bool SMIMS_FIFO_Read(USB_HANDLE dev_handle, uint16_t* ptr, int Length);

//============ Command API ==============
bool SMIMS_SyncDelay(USB_HANDLE dev_handle);
bool SMIMS_CommandActive(USB_HANDLE dev_handle);

bool SMIMS_CFGSpaceRead(USB_HANDLE dev_handle, struct SMIMS_CFGSpace* pSMIMS_CFGSpace);
bool SMIMS_CFGSpaceWrite(USB_HANDLE dev_handle, struct SMIMS_CFGSpace* pSMIMS_CFGSpace);

bool SMIMS_FPGAProgrammerActive(USB_HANDLE dev_handle);

bool SMIMS_VeriCommActive(USB_HANDLE dev_handle);
bool SMIMS_VeriInstrumentActive(USB_HANDLE dev_handle);
bool SMIMS_VeriLinkActive(USB_HANDLE dev_handle);
bool SMIMS_VeriSoCActive(USB_HANDLE dev_handle);
bool SMIMS_VeriCommProActive(USB_HANDLE dev_handle);
bool SMIMS_VeriSDKActive(USB_HANDLE dev_handle);

bool SMIMS_FlashReadActive(USB_HANDLE dev_handle);  // Flash API
bool SMIMS_FlashWriteActive(USB_HANDLE dev_handle);  // Flash API

//============ Configuration Space API ==============
uint16_t SMIMS_GetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace* pCFGSpace);
uint16_t SMIMS_GetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace* pCFGSpace);
uint8_t SMIMS_GetVeriComm_ISV(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_IsVeriComm_ClockCheck_Enable(struct SMIMS_CFGSpace* pCFGSpace);
uint8_t SMIMS_GetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace* pCFGSpace);
uint8_t SMIMS_GetModeSelector(struct SMIMS_CFGSpace* pCFGSpace);
uint16_t SMIMS_GetFlashBeginBlockAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetFlashBeginClusterAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetFlashReadEndBlockAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetFlashReadEndClusterAddr(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetSecurityKey(struct SMIMS_CFGSpace* pCFGSpace);

void SMIMS_SetVeriComm_ClockHighDelay(struct SMIMS_CFGSpace* pCFGSpace, uint16_t ClockHighDelay);
void SMIMS_SetVeriComm_ClockLowDelay(struct SMIMS_CFGSpace* pCFGSpace, uint16_t ClockLowDelay);
void SMIMS_SetVeriComm_ISV(struct SMIMS_CFGSpace* pCFGSpace, uint16_t Value);
void SMIMS_SetVeriComm_ClockCheck(struct SMIMS_CFGSpace* pCFGSpace, bool Check);
void SMIMS_SetVeriSDK_ChannelSelector(struct SMIMS_CFGSpace* pCFGSpace, uint8_t Select);
void SMIMS_SetModeSelector(struct SMIMS_CFGSpace* pCFGSpace, uint8_t Select);
void SMIMS_SetFlashBeginBlockAddr(struct SMIMS_CFGSpace* pCFGSpace, uint16_t Address);  // Flash API
void SMIMS_SetFlashBeginClusterAddr(struct SMIMS_CFGSpace* pCFGSpace, uint16_t Address);  // Flash API
void SMIMS_SetFlashReadEndBlockAddr(struct SMIMS_CFGSpace* pCFGSpace, uint16_t Address);  // Flash API
void SMIMS_SetFlashReadEndClusterAddr(struct SMIMS_CFGSpace* pCFGSpace, uint16_t Address);  // Flash API
void SMIMS_SetLicenseKey(struct SMIMS_CFGSpace* pCFGSpace, uint16_t LicenseKey);

int SMIMS_Version(struct SMIMS_CFGSpace* pCFGSpace);
int SMIMS_MajorVersion(struct SMIMS_CFGSpace* pCFGSpace);
int SMIMS_SubVersion(struct SMIMS_CFGSpace* pCFGSpace);
int SMIMS_SubSubVersion(struct SMIMS_CFGSpace* pCFGSpace);
uint16_t SMIMS_GetFIFOSize(struct SMIMS_CFGSpace* pCFGSpace);
uint16_t SMIMS_GetFlashTotalBlock(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetFlashBlockSize(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
uint16_t SMIMS_GetFlashClusterSize(struct SMIMS_CFGSpace* pCFGSpace);  // Flash API
bool SMIMS_VeriCommAbility(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_VeriInstrumentAbility(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_VeriLinkAbility(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_VeriSoCAbility(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_VeriCommProAbility(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_VeriSDKAbility(struct SMIMS_CFGSpace* pCFGSpace);

bool SMIMS_IsFPGAProgram(struct SMIMS_CFGSpace* pCFGSpace);
bool SMIMS_IsPCBConnect(struct SMIMS_CFGSpace* pCFGSpace);  // Multiple PCB API
bool SMIMS_IsVeriComm_ClockContinue(struct SMIMS_CFGSpace* pCFGSpace);

//==================================================

#endif
