#include <exception>
#include <stdexcept>

#include <QFile>

#include "FPGAManager.h"
#include "VLFDHandler.h"

using namespace rabbit_App;

FPGAManager::FPGAManager(QObject *parent) : QObject(parent) {
  vlfd_handler_ = new rabbit_vlfd::VLFDHandler();
}

FPGAManager::~FPGAManager() { delete vlfd_handler_; }

void FPGAManager::program(const QString &bitstream_path) {
  if (!QFile::exists(bitstream_path)) {
    throw std::runtime_error("Bitstream file does not exist.");
  }
  try {
    QByteArray ba = bitstream_path.toLocal8Bit();
    const char *c_str = ba.data();
    vlfd_handler_->program(c_str);
  } catch (const std::exception &e) {
    throw std::runtime_error(e.what());
  }

  emit programFinished();
}
