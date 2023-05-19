//
// VLFD V2 SDK Header File
//
#ifndef _H_SMIMS_VLFD_
#define _H_SMIMS_VLFD_

#if defined(_MSC_VER)
#ifdef VLFDLIBUSBDRIVER_EXPORTS
#define VLFDLIBRARY_API __declspec(dllexport)
#else
#define VLFDLIBRARY_API __declspec(dllimport)
#endif
#else
#define VLFDLIBRARY_API
#define WINAPI
#endif

extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_ProgramFPGA(int iBoard, const char * BitFile);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_AppOpen(int iBoard, const char * SerialNO);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_AppFIFOReadData(int iBoard, WORD *Buffer, unsigned size);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_AppFIFOWriteData(int iBoard, WORD *Buffer, unsigned size);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_AppChannelSelector(int iBoard, BYTE channel);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_AppClose(int iBoard);
extern "C" VLFDLIBRARY_API const char * WINAPI VLFD_GetLastErrorMsg(int iBoard);

extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_IO_ProgramFPGA(int iBoard, const char* BitFile);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_IO_Open(int iBoard, const char* SerialNO);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_IO_WriteReadData(int iBoard, WORD* WriteBuffer, WORD* ReadBuffer, unsigned size);
extern "C" VLFDLIBRARY_API BOOL WINAPI VLFD_IO_Close(int iBoard);
extern "C" VLFDLIBRARY_API const char* WINAPI VLFD_IO_GetLastErrorMsg(int iBoard);

#endif