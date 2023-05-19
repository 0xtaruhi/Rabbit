#ifndef _XILINX_ERROR_H_
#define _XILINX_ERROR_H_
// for VLXSP6
enum
{
    _SUCCESS_,
	_FILE_OPEN_ERROR_,
	_FILE_READ_ERROR_,
	_PART_NAME_ERROR_,
	_CLOCK_TYPE_ERROR_,
	_BITSTREAM_SIZE_ERROR_,
	_USB_DRIVER_OPEN_ERROR_,
	_USB_IO_ERROR_,
	_PID_VID_ERROR_,
	_READ_ENCRYPT_TABLE_ERROR_,
	_READ_CFG_DATA_ERROR_,
	_VERSION_ERROR_,
	_MEMORY_ALLOCATE_ERROR_,
	_SEND_CMD_ERROR_,
	_SEND_DATA_ERROR_,
	_PROGRAM_FIRST_FPGA_FAILED_,
	_PROGRAM_SECOND_FPGA_FAILED_,
	_FLASH_READ_ACTIVE_FAILED_,
	_GET_DATA_ERROR_,
	_FLASH_WRITE_ACTIVE_FAILED_,

};

static std::string errorstr[] =
{
	"Success",
	"File Open Error",
	"File Read Error",
	"Part Name Error",
	"Clock Type Error",
	"Bit-stream size Error",
	"USB Driver Can NOT be open",
	"USB IO Error",
	"Board Version Error"/*_PID_VID_ERROR*/,
	"SMIMS Internal Error"/*_READ_ENCRYPT_TABLE_ERROR_*/,
	"Read Configure Data Error",
	"Version Error",
	"Memory Allocate Error",
	"USB Command IO Error"/*_SEND_CMD_ERROR_*/,
	"USB Data IO Error",
	"Program First FPGA Design Failed",
	"Program Second FPGA Design Failed",
	"Flash Read Active Failed",
	"USB Data FIFO Read IO Error",
	"Flash Write Active Failed"
};



extern std::string errorstr[];
#endif
