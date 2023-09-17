#include "SMIMS_VLFD.h"

#include "FPGA/VLFDProgramHandler.h"

using namespace rabbit_App::fpga;

constexpr auto kNowUseBoard = 0;

VLFDProgramHandler::VLFDProgramHandler(QObject *parent) : QObject(parent) {}

VLFDProgramHandler::~VLFDProgramHandler() {}

void VLFDProgramHandler::onDownloadBitstream(const QString &bitstream_path) {
  QByteArray ba = bitstream_path.toLocal8Bit();
  const char *bitstream_path_cstr = ba.data();

  if (auto result = VLFD_ProgramFPGA(kNowUseBoard, bitstream_path_cstr);
      result != true) {
    emit downloadBitstreamFailure(VLFD_GetLastErrorMsg(kNowUseBoard));
    return;
  }
  emit downloadBitstreamSuccess();
}
