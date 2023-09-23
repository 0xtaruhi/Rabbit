#ifndef VLFD_RUNNING_HANDLER_H
#define VLFD_RUNNING_HANDLER_H

#include <array>

#include <QObject>
#include <QTimer>
#include <cstdint>
#include <stdint.h>

#include "FPGA/AsyncVLFDReadWrite.h"

namespace rabbit_App::fpga {

/// @brief VLFDRunningHandler class
/// This class is used to handle the running of the VLFD device.
class VLFDRunningHandler : public QObject {
  Q_OBJECT

public:
  VLFDRunningHandler(QObject *parent = nullptr);
  virtual ~VLFDRunningHandler();

  /// @brief This function is used to get the async vlfd read write handler.
  /// this handler is used to bind the read signal of this class to the
  /// slot of PanelUpdateController class. So that the read data can pass
  /// to the update controller thread directly.
  AsyncVLFDReadWrite *ayncVLFDReadWriteHandler() const {
    return async_vlfd_read_write_;
  }

  void setWriteData(uint64_t write_data) { write_data_ = write_data; }

public slots:
  void onStopRunning();
  void onStartRunning();
  void onFrequencyChanged(int frequency);
  void onRunningTimerTimeout();

  // void onReadWriteDone(const std::vector<uint16_t> &read_data, const
  // std::vector<uint16_t> &write_data); void onReadWriteError(const QString
  // &error_message);

signals:
  void askForWriteData();
  void doReadWrite(const std::vector<uint16_t> &write_data);
  void readWriteThreadStart();
  void readWriteThreadStop();

  // void readWriteDone(const std::vector<uint16_t> &read_data, const
  // std::vector<uint16_t> &write_data); void readWriteError(const QString
  // &error_message);

private:
  /// @brief The time split per second.
  /// Split the time into 200 parts per second.
  /// Every part, do the read/write operation.
  static constexpr auto time_split_per_sec = 200;

  QTimer *running_timer_;

  /// @brief The access usb times.
  /// This array is used to record the access usb times of the device.
  /// Decide whether to read and write the device and how many times to read
  /// and write the device.
  std::array<uint8_t, time_split_per_sec> access_usb_times_;
  AsyncVLFDReadWrite *async_vlfd_read_write_;

  uint64_t write_data_;

  /// @brief Reallocate the access usb times.
  /// The reason why we need to reallocate the access usb times is that
  /// the frequency of ayncVLFDReadWriteHandler is 200Hz, but the frequency
  /// of the device maybe larger.
  void reAllocateAccessUsbTimes(int frequency);
};

} // namespace rabbit_App::fpga

#endif // VLFD_RUNNING_HANDLER_H
