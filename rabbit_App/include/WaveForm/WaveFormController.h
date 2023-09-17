#ifndef WAVE_FORM_CONTROLLER_H
#define WAVE_FORM_CONTROLLER_H

#include "Ports/PinInfo.h"
#include "Ports/Port.h"
#include "ThreadTimer.h"
#include <QFile>
#include <QMap>


namespace rabbit_App::waveform {

enum class DataType { Read, Write }; // enum class DataType

class WaveFormController : public QObject {

  Q_OBJECT

  friend class VcdWorker;

public:
  static const QString kReadDataFileName;
  static const QString kWriteDataFileName;
  static const QString kVCDFileName;

public:
  WaveFormController(QObject *parent = nullptr);
  virtual ~WaveFormController();

  void gtkWaveExec(const QString &vcd_file_path);

  void appendData(const std::vector<uint16_t> &data, DataType data_type);

  void setPortsMap(const QVector<ports::Port> &input_ports,
                   const QVector<ports::Port> &output_ports);
  void setFrequency(const int frequency) { frequency_ = frequency; }

  void startWriting(const QString &project_path);
  void stopWriting();

private:
  void startWriting();
  void writeToVcdFile();

private:
  QFile *read_data_file_;
  QFile *write_data_file_;
  QFile *vcd_file_;

  int frequency_;
  QString project_path_;
  QMap<int, QString> input_ports_;
  QMap<int, QString> output_ports_;

  ThreadWorker *vcd_worker_;

}; // class WaveFormController

class VcdWorker : public Worker {

  Q_OBJECT

public:
  VcdWorker(WaveFormController *controller);
  ~VcdWorker();

protected:
  void doWork() override;

private:
  WaveFormController *controller_;

}; // class VcdWorker

} // namespace rabbit_App::waveform

#endif // WAVE_FORM_CONTROLLER_H