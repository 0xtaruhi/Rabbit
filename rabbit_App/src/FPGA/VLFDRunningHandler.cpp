#include <QDateTime>
#include <QDebug>
#include <cstdint>
#include <stdint.h>

#include "SMIMS_VLFD.h"

#include "FPGA/VLFDRunningHandler.h"
#include "qdebug.h"
#include "qnamespace.h"

using namespace rabbit_App::fpga;

constexpr auto kNowUseBoard = 0;
constexpr auto kSerialNo = "F4UP-G2NH-Y0M0-AC05-F805-A478";

VLFDRunningHandler::VLFDRunningHandler(QObject *parent) : QObject(parent) {
  running_timer_ = new QTimer(this);
  running_timer_->setTimerType(Qt::PreciseTimer);
  running_timer_->setInterval(1000 / time_split_per_sec);
  connect(running_timer_, &QTimer::timeout, this,
          &VLFDRunningHandler::onRunningTimerTimeout);

  async_vlfd_read_write_ = new AsyncVLFDReadWrite();
  connect(this, &VLFDRunningHandler::doReadWrite, async_vlfd_read_write_,
          &AsyncVLFDReadWrite::onDoVLFDReadWrite, Qt::QueuedConnection);

  connect(this, &VLFDRunningHandler::readWriteThreadStart,
          async_vlfd_read_write_, &AsyncVLFDReadWrite::onStartThread,
          Qt::DirectConnection);
  connect(this, &VLFDRunningHandler::readWriteThreadStop,
          async_vlfd_read_write_, &AsyncVLFDReadWrite::onStopThread,
          Qt::QueuedConnection);

  // connect(async_vlfd_read_write_, &AsyncVLFDReadWrite::readWriteDone, this,
  //         &VLFDRunningHandler::onReadWriteDone);
  // connect(async_vlfd_read_write_, &AsyncVLFDReadWrite::readWriteError, this,
  //         &VLFDRunningHandler::onReadWriteError);
}

VLFDRunningHandler::~VLFDRunningHandler() { delete async_vlfd_read_write_; }

void VLFDRunningHandler::onStopRunning() {
  emit readWriteThreadStop();
  running_timer_->stop();
  VLFD_IO_Close(kNowUseBoard);
  // qDebug() << "stop ,current time :"
  //          << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
}

void VLFDRunningHandler::onStartRunning() {
  emit readWriteThreadStart();
  running_timer_->start();
  VLFD_IO_Open(kNowUseBoard, kSerialNo);
  // qDebug() << "start ,current time :"
  //          << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
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
