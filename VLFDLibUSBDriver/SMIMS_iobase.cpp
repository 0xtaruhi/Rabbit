#include "pch.h"
#include "SMIMS_iobase.h"


USB_HANDLE usb_device_open(uint16_t VID, uint16_t PID, int devnum)
{
	//truct usb_bus *busses, *bus;
	//struct usb_device *dev;
	//usb_dev_handle *dev_handle;
	libusb_device_handle * dev_handle;
	//bool find_next;
	//unsigned int devcnt = 0;

	libusb_init(NULL);

#ifdef DEBUG
	libusb_device** devs;
	ssize_t devs_cnt = libusb_get_device_list(NULL, &devs);

	for (size_t idx = 0; idx < devs_cnt; ++idx) {
		libusb_device *device = devs[idx];
		libusb_device_descriptor desc = {0};

		libusb_get_device_descriptor(device, &desc);

		printf("Vendor: Device = %04x:%04x\n", desc.idVendor, desc.idProduct);
	}
#endif
	dev_handle = libusb_open_device_with_vid_pid(NULL, VID, PID);

	if(dev_handle == NULL)
		return NULL;

/*
	usb_find_busses();
	usb_find_devices();

	busses = usb_get_busses();

	find_next = true;
	bus = busses;
	while(find_next && bus)
	{
		dev = bus->devices;
		while(find_next && dev)
		{
			if((dev->descriptor.idVendor == VID) && (dev->descriptor.idProduct == PID))
			{
				if(devcnt == devnum)
					find_next = false;
				else
				{
					devcnt = devcnt + 1;
					dev = dev->next;
				}
			}
			else
				dev = dev->next;
		}

		if(find_next)
			bus = bus->next;
	}

	if(dev == NULL)
		return NULL;

	dev_handle = usb_open(dev);
	if(dev_handle == NULL)
		return NULL;

	if(libusb_set_configuration(dev_handle, dev->config[0].bConfigurationValue) < 0)
	{
		usb_device_close(dev_handle);
		return NULL;
	}
*/
	//libusb_set_configuration(dev_handle, 1);

	if(libusb_claim_interface(dev_handle, 0) < 0)
	{
		usb_device_close(dev_handle);
		return NULL;
	}
/*
	if(usb_set_altinterface(dev_handle, 0) < 0)
	{
		usb_device_close(dev_handle);
		return NULL;
	}
*/
	if(libusb_clear_halt(dev_handle, EP2) < 0)  // Clear EP2 OUT
	{
		usb_device_close(dev_handle);
		return NULL;
	}

	if(libusb_clear_halt(dev_handle, EP4) < 0)  // Clear EP4 OUT
	{
		usb_device_close(dev_handle);
		return NULL;
	}

	if(libusb_clear_halt(dev_handle, EP6) < 0)  // Clear EP6 IN
	{
		usb_device_close(dev_handle);
		return NULL;
	}

	if(libusb_clear_halt(dev_handle, EP8) < 0)  // Clear EP8 IN
	{
		usb_device_close(dev_handle);
		return NULL;
	}

	return dev_handle;
}

bool usb_device_close(USB_HANDLE dev_handle)
{
	if(dev_handle == NULL)
		return false;

	if(libusb_release_interface(dev_handle, 0) < 0)
		return false;

	libusb_close(dev_handle);

	return true;
}

int usb_get_device_descriptor(USB_HANDLE dev_handle, struct libusb_device_descriptor *dev_dscr)
{
	libusb_device * dev;
	dev = libusb_get_device(dev_handle);
	//return libusb_get_descriptor(dev_handle, LIBUSB_DT_DEVICE, 0, dev_dscr, LIBUSB_DT_DEVICE_SIZE);
	return libusb_get_device_descriptor(dev, dev_dscr);
}

bool SMIMS_WriteUSB(USB_HANDLE dev_handle, int EndPoint, void *Buffer, int Size)
{
	char *ptr = (char *)Buffer;
	int WriteSize;
	bool bResult = true;
	int iRet;

	while((Size > 0) && bResult)
	{
		iRet = libusb_bulk_transfer(dev_handle, EndPoint, (unsigned char *)ptr, Size, &WriteSize, 1000);

		switch(iRet)
		{
		default:
			bResult = false;
		break;
		case 0:	//success(and populates transferred)
			bResult = true;
		break;
		case LIBUSB_ERROR_TIMEOUT:	//if the transfer timed out (and populates transferred)
			bResult = false;
		break;
		case LIBUSB_ERROR_PIPE:	//if the endpoint halted
			bResult = false;
		break;
		case LIBUSB_ERROR_OVERFLOW:	//if the device offered more data
			bResult = false;
		break;
		case LIBUSB_ERROR_NO_DEVICE:	//if the device has been disconnected
            bResult = false;
		break;
		}
		if (bResult)
		{
			if(WriteSize < 0)
				bResult = false;
			else
			{
				Size -= WriteSize;
				ptr += WriteSize;

			}
		}
		else
			break;

		/*
		WriteSize = usb_bulk_write(dev_handle, EndPoint, ptr, Size, 0);
		if(WriteSize < 0)
			bResult = false;
		else
		{
			Size -= WriteSize;
			ptr += WriteSize;
		}
		*/
	}
	return bResult;
}

bool SMIMS_ReadUSB(USB_HANDLE dev_handle, int EndPoint, void *Buffer, int Size)
{
	char *ptr = (char *)Buffer;
	int ReadSize;
	bool bResult = true;
	int iRet;

    ReadSize = 0;
	while((Size > 0) && bResult)
	{
		iRet = libusb_bulk_transfer(dev_handle, EndPoint, (unsigned char *)ptr, Size, &ReadSize, 1000);

		switch(iRet)
		{
		default:
			bResult = false;
		break;
		case 0:	//success(and populates transferred)
			bResult = true;
		break;
		case LIBUSB_ERROR_TIMEOUT:	//if the transfer timed out (and populates transferred)
			bResult = false;
			//printf("timeout\n");
		break;
		case LIBUSB_ERROR_PIPE:	//if the endpoint halted
            bResult = false;
			//printf("pipe\n");
		break;
		case LIBUSB_ERROR_OVERFLOW:	//if the device offered more data
            bResult = false;
			//printf("overflow\n");
		break;
		case LIBUSB_ERROR_NO_DEVICE:	//if the device has been disconnected
            bResult = false;
			//printf("nodevice\n");
		break;
		}
		if (bResult)
		{
			if(ReadSize < 0)
				bResult = false;
			else
			{
				Size -= ReadSize;
				ptr += ReadSize;
			}
		}
		else
			break;



		/*
		ReadSize = usb_bulk_read(dev_handle, EndPoint, ptr, Size, 0);
		if(ReadSize < 0)
			bResult = false;
		else
		{
			Size -= ReadSize;
			ptr += ReadSize;
		}
		*/
	}
	return bResult;
}
