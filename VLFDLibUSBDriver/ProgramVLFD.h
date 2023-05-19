//---------------------------------------------------------------------
// FILE: ProgramVLFD.h
// AUTHOR: Yi-Huei Chung
// ABSTRATE:
// FUNCTION:
// MODIFICATION HISTORY:
//	2009.07.21 Create File.
//
//  (c) copyright 2005 SMIMS Technology Corp. All rights reserved
//---------------------------------------------------------------------

#ifndef SMIMSVLXFDProgrammerH
#define SMIMSVLXFDProgrammerH

#include <iostream>
#include <fstream>
#include "Program_error.h"
#include "SMIMS_API.h"

#define   SMIMS_VERSION   0x0220
#define   DW_VID   0x2200
#define   DW_PID   0x2008

#define FUDAN_FPGA_NUM 2

#define MAX_TIMEOUT     20000
#define POLLING_PERIOD  200
#ifdef	_DEBUG
#define		OUTPUT_DEBUG_MSG
#endif

class TSMIMSVLFDProgrammer
{
private:
	char ErrorMsg[128];

	std::fstream fi;
	USB_HANDLE hDevice;
    WORD BlockSize;
	WORD *Buffer;
	//char g_szUSBDev[128];

	struct SMIMS_CFGSpace CFGSpace, EncryptCFG;
    WORD EncryptTable[32];
	unsigned encindex, decindex;

    unsigned j;
	unsigned BSLen;
	// unsigned BlockLen;

	int ProgramFPGA(int iBoard, const char * g_szBitFile);
    // int ProgramFPGA_windows(const char * g_szBitFile);

public:
	int g_iErrCode;
    int g_iProgress;

	TSMIMSVLFDProgrammer(void);
	bool StartProgram(int iBoard, const char * g_szBitFile1);
	char * GetLastErrorMsg(void);
};
#endif //SMIMSVLXFDProgrammerH
