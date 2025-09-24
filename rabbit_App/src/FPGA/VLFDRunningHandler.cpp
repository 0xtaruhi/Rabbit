#include <QDateTime>
#include <QDebug>
#include <cstdint>
#include <stdexcept>
#include <stdint.h>

#include "FPGA/VLFDFFI.h"

#include "FPGA/VLFDRunningHandler.h"
#include "qnamespace.h"

using namespace rabbit_App::fpga;

VLFDRunningHandler::VLFDRunningHandler(VlfdDevice *&device, QObject *parent)
    : QObject(parent), device_(device) {
  running_timer_ = new QTimer(this);
  running_timer_->setTimerType(Qt::PreciseTimer);
  running_timer_->setInterval(1000 / time_split_per_sec);
  connect(running_timer_, &QTimer::timeout, this,
          &VLFDRunningHandler::onRunningTimerTimeout);

  async_vlfd_read_write_ = new AsyncVLFDReadWrite(device_);
  connect(this, &VLFDRunningHandler::doReadWrite, async_vlfd_read_write_,
          &AsyncVLFDReadWrite::onDoVLFDReadWrite, Qt::QueuedConnection);

  connect(this, &VLFDRunningHandler::readWriteThreadStart,
          async_vlfd_read_write_, &AsyncVLFDReadWrite::onStartThread,
          Qt::DirectConnection);
  connect(this, &VLFDRunningHandler::readWriteThreadStop,
          async_vlfd_read_write_, &AsyncVLFDReadWrite::onStopThread,
          Qt::QueuedConnection);
}

VLFDRunningHandler::~VLFDRunningHandler() { delete async_vlfd_read_write_; }

void VLFDRunningHandler::onStopRunning() {
  if (vlfd_io_close(device_) != 0) {
    throw std::runtime_error("FPGA Close failed" +
                             std::string(vlfd_get_last_error_message()));
  }
  emit readWriteThreadStop();
  running_timer_->stop();
}

void VLFDRunningHandler::onStartRunning() {
  device_ = vlfd_io_open();
  if (device_ == nullptr) {
    throw std::runtime_error("FPGA Open failed" +
                             std::string(vlfd_get_last_error_message()));
  }
  emit readWriteThreadStart();
  running_timer_->start();
}

void VLFDRunningHandler::onFrequencyChanged(int frequency) {
  reAllocateAccessUsbTimes(frequency);
}

void VLFDRunningHandler::onRunningTimerTimeout() {
  static int index = 0;
  index = (index + 1) % time_split_per_sec;
  std::vector<uint16_t> data;
  emit askForWriteData();
  for (uint8_t times = 0; times != access_usb_times_[index]; ++times) {
    // data.push_back(0x0001);
    // data.push_back(0x0000);
    // data.push_back(0x0000);
    // data.push_back(0x0000);
    // split write_data_ into 4 uint16_t and push into data
    for (int i = 0; i < 4; i++) {
      data.push_back((write_data_ >> (i * 16)) & 0xFFFF);
    }
  }
  if (!data.empty()) {
    emit doReadWrite(data);
  }
}

void VLFDRunningHandler::reAllocateAccessUsbTimes(int frequency) {
  access_usb_times_.fill(0);
  uint8_t base_times = frequency / time_split_per_sec;
  uint8_t remainder_times = frequency % time_split_per_sec;

  if (remainder_times != 0) {
    int step = time_split_per_sec / remainder_times;
    for (int i = 0; i != remainder_times; ++i) {
      access_usb_times_[i * step] = base_times + 1;
    }
  }

  for (int i = 0; i != time_split_per_sec; ++i) {
    if (access_usb_times_[i] == 0) {
      access_usb_times_[i] = base_times;
    }
  }
}

// void VLFDRunningHandler::onReadWriteDone(
//     const std::vector<uint16_t> &read_data, const std::vector<uint16_t>
//     &write_data) {
//   emit readWriteDone(std::move(read_data), std::move(write_data));
// }

// void VLFDRunningHandler::onReadWriteError(const QString &error_message) {
//   emit readWriteError(error_message);
// }
