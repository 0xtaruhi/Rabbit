#ifndef WAVE_FORM_CONTROLLER_H
#define WAVE_FORM_CONTROLLER_H

#include "Ports/PinInfo.h"
#include "Ports/Port.h"
#include "ThreadTimer.h"
#include <QFile>
#include <QMap>

namespace rabbit_App::waveform {

/// @brief Enum class of the waveform data type.
enum class DataType { Read, Write }; // enum class DataType

/// @brief WaveFormController class
/// This class implements all the waveform function, including
/// Read/Write data collection, vcd file generation, gtkwave process
/// This class use a worker to avoid gui blocking when write to vcd files.
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

  /// @brief Execute gtkwave. Platform related.
  /// @param vcd_file_path the path of the vcd file
  void gtkWaveExec(const QString &vcd_file_path);

  /// @brief Append a data to data file.
  /// @param data the data to write
  /// @param data_type Read or Write
  void appendData(const std::vector<uint16_t> &data, DataType data_type);

  /// @brief Set the ports map from Project Manager.
  void setPortsMap(const QVector<ports::Port> &input_ports,
                   const QVector<ports::Port> &output_ports);
  /// @brief Set the frequency. Frequency is used in vcd file.
  void setFrequency(const int frequency) { frequency_ = frequency; }

  /// @brief start data file writing
  void startWriting(const QString &project_path);

  /// @brief stop data file writing
  void stopWriting();

  void setWaveformEnabled(bool enabled);
  bool isWaveformEnabled() const noexcept { return waveform_enabled_; }

private:
  void startWriting();

  /// @brief Convert the data file to vcd file.
  /// The data file may be very long, so this function may take a long time.
  /// For not blocking the gui thread, do this function in the worker.
  void writeToVcdFile();

private:
  /// @brief read data file
  QFile *read_data_file_;

  /// @brief write data file
  QFile *write_data_file_;

  /// @brief vcd file
  QFile *vcd_file_;

  int frequency_;
  QString project_path_;

  /// @brief input ports map
  QMap<int, QString> input_ports_;

  /// @brief output ports map
  QMap<int, QString> output_ports_;

  /// @brief worker manager for write to vcd file
  ThreadWorker *vcd_worker_;

  bool waveform_enabled_;

}; // class WaveFormController

/// @brief VcdWorker class, used to write data to vcd file.
class VcdWorker : public Worker {

  Q_OBJECT

public:
  VcdWorker(WaveFormController *controller);
  ~VcdWorker();

protected:
  /// @brief Write data to vcd file.
  void doWork() override;

private:
  WaveFormController *controller_;

}; // class VcdWorker

} // namespace rabbit_App::waveform

#endif // WAVE_FORM_CONTROLLER_H
