#ifndef FPGA_MANAGER_H
#define FPGA_MANAGER_H

#include <QObject>

#include "VLFDHandler.h"

namespace rabbit_App {

class FPGAManager : public QObject {
  Q_OBJECT

public:
  FPGAManager(QObject *parent = nullptr);
  ~FPGAManager();

  void program(const QString& bitstream_path);

signals:
  void programFinished();

private:
  rabbit_vlfd::VLFDHandler *vlfd_handler_;
};

} // namespace rabbit_App

#endif // FPGA_MANAGER_H
