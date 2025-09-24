#include <cstddef>
#include <cstdint>
#include <stdint.h>

#include <QApplication>
#include <QMetaObject>
#include <vector>

#include "FPGA/AsyncVLFDReadWrite.h"
#include "FPGA/VLFDFFI.h"
using namespace rabbit_App::fpga;

constexpr auto kBufferLength = 1024;

static std::vector<uint16_t> convertToVector(const uint16_t *data,
                                             const int length) {
  std::vector<uint16_t> result;
  result.reserve(length);
  for (auto i = 0; i < length; ++i) {
    result.push_back(data[i]);
  }
  return result;
}

AsyncVLFDReadWrite::AsyncVLFDReadWrite(VlfdDevice *&device, QObject *parent)
    : QObject(parent), device_(device) {
  thread_ = new QThread(this);
  thread_->setObjectName("RabbitUSBRW");
  is_running_ = false;
  write_buf_ = new uint16_t[kBufferLength];
  read_buf_ = new uint16_t[kBufferLength];
}

AsyncVLFDReadWrite::~AsyncVLFDReadWrite() {
  delete[] write_buf_;
  delete[] read_buf_;
}

void AsyncVLFDReadWrite::checkConnection() {
  for (auto i = 0; i < kBufferLength; ++i) {
    write_buf_[i] = 0;
    read_buf_[i] = 0;
  }
  vlfd_io_write_read(device_, write_buf_, read_buf_, 4);
}

void AsyncVLFDReadWrite::onStartThread() {
  moveToThread(thread_);
  thread_->start();
  // thread_worker_->start();
  is_running_ = true;
  checkConnection();
}

void AsyncVLFDReadWrite::onStopThread() {
  // moveToThread(QApplication::instance()->thread());
  thread_->quit();
  thread_->wait();
  // thread_worker_->stop();
  is_running_ = false;
}

void AsyncVLFDReadWrite::onDoVLFDReadWrite(
    const std::vector<uint16_t> &write_data) {
  // qDebug() << "AsyncVLFDReadWrite::onDoVLFDReadWrite() thread: "
  //          << QThread::currentThreadId();
  const auto data_size = write_data.size();
  if (is_running_) {
    updateWriteBuffer(write_data);

    auto result = vlfd_io_write_read(device_, write_buf_, read_buf_, data_size);

    if (result == 0) {
      auto read_data = convertToVector(read_buf_, data_size);
      emit readWriteDone(std::move(read_data), std::move(write_data));
      // emit readWriteDone(std::move(read_data));
    } else {
      emit readWriteError(vlfd_get_last_error_message());
    }
  } else {
    // qDebug() << "VLFD read write thread is not running";
  }
}

void AsyncVLFDReadWrite::updateWriteBuffer(
    const std::vector<uint16_t> &write_data) {
  for (size_t i = 0; i < write_data.size(); ++i) {
    write_buf_[i] = write_data[i];
  }
}
