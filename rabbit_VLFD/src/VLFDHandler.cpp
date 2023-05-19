#include "VLFDHandler.h"
#include "SMIMS_VLFD.h"

constexpr auto kSerialNo = "F4UP-G2NH-Y0M0-AC05-F805-A478";
constexpr auto kNowUseBoard = 0;

using namespace rabbit_vlfd;

VLFDHandler::VLFDHandler() : is_programmed_(false) {}

VLFDHandler::~VLFDHandler() {}

void VLFDHandler::program(const char *bit_file_path) {
  if (auto result = VLFD_ProgramFPGA(kNowUseBoard, bit_file_path);
      result != true) {
    throw std::runtime_error(VLFD_GetLastErrorMsg(kNowUseBoard));
  }
  is_programmed_ = true;
}

void VLFDHandler::initIO(int write_count, int read_count) noexcept {
  write_buf_.resize(write_count);
  read_buf_.resize(read_count);
}

void VLFDHandler::openIO() {
  if (auto result = VLFD_IO_Open(kNowUseBoard, kSerialNo); result != true) {
    throw std::runtime_error(VLFD_GetLastErrorMsg(kNowUseBoard));
  }
}

void VLFDHandler::closeIO() {
  if (auto result = VLFD_IO_Close(kNowUseBoard); result != true) {
    throw std::runtime_error(VLFD_GetLastErrorMsg(kNowUseBoard));
  }
}

void VLFDHandler::writeReadData() {
  //! Confused. 
  //  Should it be write_buf_.size() or read_buf_.size()? 
  //  Or should it be the max of the two?
  //  In wonton's implementation, they both equal to 4.
  auto size = write_buf_.size();

  if (auto result = VLFD_IO_WriteReadData(kNowUseBoard, write_buf_.data(),
                                          read_buf_.data(), size);
      result != true) {
    throw std::runtime_error(VLFD_GetLastErrorMsg(kNowUseBoard));
  }
}
