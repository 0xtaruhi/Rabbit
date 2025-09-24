#include "vlfd_ffi.h"

#include "FPGA/VLFDProgramHandler.h"

using namespace rabbit_App::fpga;

VLFDProgramHandler::VLFDProgramHandler(QObject *parent) : QObject(parent) {}

VLFDProgramHandler::~VLFDProgramHandler() {}

void VLFDProgramHandler::onDownloadBitstream(const QString &bitstream_path) {
  QByteArray ba = bitstream_path.toLocal8Bit();
  const char *bitstream_path_cstr = ba.data();

  if (auto result = vlfd_program_fpga(bitstream_path_cstr); result != 0) {
    emit downloadBitstreamFailure(vlfd_get_last_error_message());
    return;
  }
  emit downloadBitstreamSuccess();
}
