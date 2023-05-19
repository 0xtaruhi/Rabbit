//
// VLFD V2 SDK Header File
//
#ifndef _H_SMIMS_VLFD_
#define _H_SMIMS_VLFD_

extern "C" bool WINAPI VLFD_ProgramFPGA(int iBoard, const char * BitFile);
extern "C" bool WINAPI VLFD_AppOpen(int iBoard, const char * SerialNO);
extern "C" bool WINAPI VLFD_AppFIFOReadData(int iBoard, uint16_t *Buffer, unsigned size);
extern "C" bool WINAPI VLFD_AppFIFOWriteData(int iBoard, uint16_t *Buffer, unsigned size);
extern "C" bool WINAPI VLFD_AppChannelSelector(int iBoard, BYTE channel);
extern "C" bool WINAPI VLFD_AppClose(int iBoard);
extern "C" const char * WINAPI VLFD_GetLastErrorMsg(int iBoard);

extern "C" bool WINAPI VLFD_IO_ProgramFPGA(int iBoard, const char* BitFile);
extern "C" bool WINAPI VLFD_IO_Open(int iBoard, const char* SerialNO);
extern "C" bool WINAPI VLFD_IO_WriteReadData(int iBoard, uint16_t* WriteBuffer, uint16_t* ReadBuffer, unsigned size);
extern "C" bool WINAPI VLFD_IO_Close(int iBoard);
extern "C" const char* WINAPI VLFD_IO_GetLastErrorMsg(int iBoard);

#endif