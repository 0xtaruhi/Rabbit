
#ifndef H_SMIMS_IOBASE
#define H_SMIMS_IOBASE

#include "SMIMS_define.h"
#include <libusb.h>


//The address of the endpoint described by this descriptor.
//Bits 0:3 are the endpoint number. Bits 4:6 are reserved. Bit 7 indicates direction
//#define EP2 (LIBUSB_ENDPOINT_OUT | 2)
//#define EP6 (LIBUSB_ENDPOINT_IN| 6)
#define EP2 0x02  // FIFO_WR
#define EP4 0x04  // COMMAND_WR
#define EP6 0x86  // FIFO_RD
#define EP8 0x88  // sync result

// #define EP2 2  // FIFO_WR
// #define EP4 4  // COMMAND_WR
// #define EP6 6  // FIFO_RD
// #define EP8 8  // sync result



USB_HANDLE usb_device_open(uint16_t VID, uint16_t PID, int devnum);
bool usb_device_close(USB_HANDLE dev_handle);
int usb_get_device_descriptor(USB_HANDLE dev_handle, struct libusb_device_descriptor* dev_dscr);

bool SMIMS_WriteUSB(USB_HANDLE dev_handle, int EndPoint, void *Buffer, int Size);
bool SMIMS_ReadUSB(USB_HANDLE dev_handle, int EndPoint, void *Buffer, int Size);

#endif
