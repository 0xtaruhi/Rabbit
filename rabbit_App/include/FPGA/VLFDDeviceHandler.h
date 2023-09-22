#ifndef VLFD_DEVICE_HANDLER_H
#define VLFD_DEVICE_HANDLER_H

#include <QObject>
#include <QThread>
#include <cstdint>

#include "FPGA/AsyncVLFDReadWrite.h"
#include "FPGA/VLFDProgramHandler.h"
#include "FPGA/VLFDRunningHandler.h"
#include "qglobal.h"

namespace rabbit_App::fpga {

/// @brief VLFDDeviceHandler class
/// This class is used to handle the VLFD device,
/// including the program and the running.
class VLFDDeviceHandler : public QObject {
  Q_OBJECT

public:
  VLFDDeviceHandler(QObject *parent = nullptr);
  virtual ~VLFDDeviceHandler();

  /// @brief Program the bitstream to the FPGA.
  void program(const QString &bitstream_path);

  /// @brief This function is used to get the async vlfd read write handler.
  /// See VLFDRunningHandler.h for more details.
  AsyncVLFDReadWrite *ayncVLFDReadWriteHandler() const {
    return running_handler_->ayncVLFDReadWriteHandler();
  }

  /// @brief Set the write data in the running handler.
  void setWriteData(uint64_t write_data) {
    running_handler_->setWriteData(write_data);
  }

signals:
  void downloadBitstreamSuccess();
  void downloadBitstreamFailure(const QString &error_message);
  void startRunning();
  void stopRunning();
  void readWriteDone(const std::vector<uint16_t> &read_data,
                     const std::vector<uint16_t> &write_data);
  void readWriteError(const QString &error_message);
  void askForWriteData();

public slots:
  void forwardDownloadBitstreamSuccess() { emit downloadBitstreamSuccess(); }
  void forwardDownloadBitstreamFailure(const QString &error_message) {
    emit downloadBitstreamFailure(error_message);
  }
  void onStopRunning() { emit stopRunning(); }
  void onStartRunning() { emit startRunning(); }
  void onFrequencyChanged(int frequency) {
    running_handler_->onFrequencyChanged(frequency);
  }
  // void onReadWriteDone(const std::vector<uint16_t> &read_data, const
  // std::vector<uint16_t> &write_data); void onReadWriteError(const QString
  // &error_message);

private:
  /// @brief The program handler.
  VLFDProgramHandler *program_handler_;
  /// @brief The running handler.
  VLFDRunningHandler *running_handler_;
};

} // namespace rabbit_App::fpga

#endif // VLFD_DEVICE_HANDLER_H
