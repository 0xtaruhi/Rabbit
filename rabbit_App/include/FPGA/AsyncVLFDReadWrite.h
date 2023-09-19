#pragma once
#include <cstdint>
#ifndef ASYNC_VLFD_READ_WRITE_H
#define ASYNC_VLFD_READ_WRITE_H

#include <array>
#include <vector>

#include <QObject>
#include <QThread>

namespace rabbit_App::fpga {

class AsyncVLFDReadWrite : public QObject {
  Q_OBJECT

public:
  AsyncVLFDReadWrite(QObject *parent = nullptr);
  ~AsyncVLFDReadWrite();

signals:
  void readWriteDone(const std::vector<uint16_t> &read_data,
                     const std::vector<uint16_t> &write_data);
  void readWriteError(const QString &error_message);

public slots:
  void onDoVLFDReadWrite(const std::vector<uint16_t> &write_data);

  void onStartThread();
  void onStopThread();

private:
  QThread *thread_;
  bool is_running_;

  uint16_t *write_buf_;
  uint16_t *read_buf_;

  void updateWriteBuffer(const std::vector<uint16_t> &write_data);
};

} // namespace rabbit_App::fpga

#endif
