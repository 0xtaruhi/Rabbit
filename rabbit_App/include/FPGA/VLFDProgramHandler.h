#ifndef VLFD_PROGRAM_HANDLER_H
#define VLFD_PROGRAM_HANDLER_H

#include <QObject>
#include <QString>

namespace rabbit_App::fpga {

/// @brief VLFDProgramHandler class
/// This class is used to handle the program of the VLFD device.
class VLFDProgramHandler : public QObject {
  Q_OBJECT

public:
  VLFDProgramHandler(QObject *parent = nullptr);
  virtual ~VLFDProgramHandler();

signals:
  void downloadBitstreamSuccess();
  void downloadBitstreamFailure(const QString &error_message);

public slots:
  void onDownloadBitstream(const QString &bitstream_path);
};

} // namespace rabbit_App::fpga

#endif // VLFD_PROGRAM_HANDLER_H
