#pragma once
#include <cstdint>
#ifndef ASYNC_VLFD_READ_WRITE_H
#define ASYNC_VLFD_READ_WRITE_H

#include <array>
#include <vector>

#include <QObject>
#include <QThread>

#include "FPGA/VLFDFFI.h"

namespace rabbit_App::fpga {

/// @brief AsyncVLFDReadWrite class
/// This class is used to process the read/write of the FPGA.
class AsyncVLFDReadWrite : public QObject {
  Q_OBJECT

public:
  AsyncVLFDReadWrite(VlfdDevice *&device, QObject *parent = nullptr);
  ~AsyncVLFDReadWrite();

signals:
  /// @brief Signal emitted when the read/write is done.
  /// Pass the read data and write data to waveform controller.
  /// @param read_data The read data.
  /// @param write_data The write data.
  void readWriteDone(const std::vector<uint16_t> &read_data,
                     const std::vector<uint16_t> &write_data);

  /// @brief Signal emitted when the read/write is failed.
  /// Pass the error message to main windows.
  /// @param error_message The error message.
  void readWriteError(const QString &error_message);

public slots:
  /// @brief Slot called when running handler ask to start processing.
  void onDoVLFDReadWrite(const std::vector<uint16_t> &write_data);

  /// @brief Slot called when running handler ask to start processing.
  void onStartThread();

  /// @brief Slot called when running handler ask to stop processing.
  void onStopThread();

private:
  /// @brief Check the connection of the FPGA.
  /// This function is called when the thread is started.
  /// When the read/write start, the first cycle of read/write is uncertain.
  /// So send a blank data to the FPGA to reflect and avoid uncertain data.
  void checkConnection();

private:
  QThread *thread_;
  bool is_running_;

  /// @brief The write buffer.
  uint16_t *write_buf_;
  /// @brief The read buffer.
  uint16_t *read_buf_;

  VlfdDevice *&device_;

  /// @brief Update the write buffer.
  /// @param write_data The write data.
  void updateWriteBuffer(const std::vector<uint16_t> &write_data);
};

} // namespace rabbit_App::fpga

#endif
