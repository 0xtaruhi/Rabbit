
#ifndef H_SMIMS_DEFINE
#define H_SMIMS_DEFINE

#if defined(_MSC_VER)

#else

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef unsigned char BOOL;
typedef int INT;
typedef unsigned int UINT;
typedef char CHAR;
typedef void VOID;

#define TRUE 1
#define FALSE 0
#endif

typedef struct libusb_device_handle * USB_HANDLE;

#endif
