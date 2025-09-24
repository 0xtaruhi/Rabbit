#include "FPGA/VLFDDeviceHandler.h"
#include "FPGA/VLFDRunningHandler.h"
#include <cstdint>

using namespace rabbit_App::fpga;

VLFDDeviceHandler::VLFDDeviceHandler(QObject *parent)
    : QObject(parent), program_handler_(new VLFDProgramHandler(this)),
      running_handler_(new VLFDRunningHandler(device_)) {
  connect(program_handler_, &VLFDProgramHandler::downloadBitstreamSuccess, this,
          &VLFDDeviceHandler::forwardDownloadBitstreamSuccess);
  connect(program_handler_, &VLFDProgramHandler::downloadBitstreamFailure, this,
          &VLFDDeviceHandler::forwardDownloadBitstreamFailure);

  connect(this, &VLFDDeviceHandler::startRunning, running_handler_,
          &VLFDRunningHandler::onStartRunning);
  connect(this, &VLFDDeviceHandler::stopRunning, running_handler_,
          &VLFDRunningHandler::onStopRunning);

  //   connect(running_handler_, &VLFDRunningHandler::readWriteDone, this,
  //           &VLFDDeviceHandler::onReadWriteDone);
  //   connect(running_handler_, &VLFDRunningHandler::readWriteError, this,
  //           &VLFDDeviceHandler::readWriteError);
  connect(running_handler_, &VLFDRunningHandler::askForWriteData, this,
          [this]() { emit askForWriteData(); });
}

VLFDDeviceHandler::~VLFDDeviceHandler() {}

void VLFDDeviceHandler::program(const QString &bitstream_path) {
  program_handler_->onDownloadBitstream(bitstream_path);
}

// void VLFDDeviceHandler::onReadWriteDone(
//     const std::vector<uint16_t> &read_data, const std::vector<uint16_t>
//     &write_data) {
//   emit readWriteDone(std::move(read_data), std::move(write_data));
// }

// void VLFDDeviceHandler::onReadWriteError(const QString &error_message) {
//   emit readWriteError(error_message);
// }
