#include "WaveForm/WaveFormController.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
// #include <QtSystemDetection>

using namespace rabbit_App::waveform;

const QString WaveFormController::kReadDataFileName = "RabbitReadData.txt";
const QString WaveFormController::kWriteDataFileName = "RabbitWriteData.txt";
const QString WaveFormController::kVCDFileName = "RabbitWaveForm.vcd";

WaveFormController::WaveFormController(QObject *parent) : QObject(parent) {
  read_data_file_ = new QFile(this);
  write_data_file_ = new QFile(this);
  vcd_file_ = new QFile(this);
  vcd_worker_ =
      new ThreadWorker(new VcdWorker(this), this, Qt::QueuedConnection);
  vcd_worker_->start();
}

WaveFormController::~WaveFormController() { vcd_worker_->stop(); }

void WaveFormController::gtkWaveExec(const QString &project_path) {
  QFileInfo project_info(project_path);
  project_path_ = project_info.absolutePath();
  auto vcd_file_path = project_path_ + "/" + kVCDFileName;
  QProcess *process = new QProcess(this);

#ifdef Q_OS_MAC
  // auto gtkwave_path = QCoreApplication::applicationDirPath() +
  // "/gtkwave/gtkwave.app/Contents/MacOS/gtkwave";
  process->start("open", QStringList() << vcd_file_path);
#else
  process->start("./gtkwave/bin/gtkwave", QStringList() << vcd_file_path);
#endif



  // process->start("./gtkwave/bin/gtkwave", QStringList() << vcd_file_path);
  // qDebug() << "vcf_file_path" << vcd_file_path;
  // qDebug() << "gtkwave_path" << gtkwave_path;
  // qDebug() << process->readAllStandardError();
  // qDebug() << process->readAllStandardOutput();
}

void WaveFormController::startWriting() {
  if (!read_data_file_->open(QIODevice::WriteOnly | QIODevice::Text) ||
      !write_data_file_->open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open files to write waveform data.");
  }
}

void WaveFormController::startWriting(const QString &project_path) {
  QFileInfo project_info(project_path);
  project_path_ = project_info.absolutePath();
  read_data_file_->setFileName(project_path_ + "/" + kReadDataFileName);
  write_data_file_->setFileName(project_path_ + "/" + kWriteDataFileName);
  startWriting();
}

void WaveFormController::stopWriting() {
  read_data_file_->close();
  write_data_file_->close();
  // writeToVcdFile();
  vcd_worker_->work();
}

void WaveFormController::appendData(const std::vector<uint16_t> &data,
                                    DataType data_type) {
  auto &file = data_type == DataType::Read ? read_data_file_ : write_data_file_;
  for (auto i = 0; i < static_cast<int>(data.size()); i += 4) {
    // fill to 16 bits
    for (auto j = 3; j >= 0; j--) {
      auto str = QString::number(data[i + j], 2);
      while (str.size() < 16) {
        str = "0" + str;
      }
      file->write(str.toUtf8());
    }
    file->write("\n");
  }
}

void WaveFormController::setPortsMap(const QVector<ports::Port> &input_ports,
                                     const QVector<ports::Port> &output_ports) {
  input_ports_.clear();
  output_ports_.clear();
  for (auto &port : input_ports) {
    input_ports_[ports::inputDeclIndexMap(port.pin_name)] = port.name;
  }
  for (auto &port : output_ports) {
    output_ports_[ports::outputDeclIndexMap(port.pin_name) - 1] = port.name;
  }
}

void WaveFormController::writeToVcdFile() {
  // qDebug() << "WaveFormController::writeToVcdFile() thread: "
  //          << QThread::currentThreadId();
  vcd_file_->setFileName(project_path_ + "/" + kVCDFileName);
  if (!vcd_file_->open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open files to write waveform data.");
  }
  // time scale
  QString time_scale =
      frequency_ < 1000
          ? QString::number((double)(1000 / frequency_)).toUtf8() + " ms"
          : QString::number((double)(1000000 / frequency_)).toUtf8() + " ns";
  vcd_file_->write("$timescale " + time_scale.toUtf8() + "\n$end\n");

  // ports declaration
  for (auto &port : input_ports_) {
    vcd_file_->write("$var wire 1 " + port.toUtf8() + " " + port.toUtf8() +
                     " $end\n");
  }
  for (auto &port : output_ports_) {
    vcd_file_->write("$var wire 1 " + port.toUtf8() + " " + port.toUtf8() +
                     " $end\n");
  }
  vcd_file_->write("$enddefinitions $end\n");

  // dumpvars
  vcd_file_->write("$dumpvars\n");
  for (auto &port : input_ports_) {
    vcd_file_->write("b0 " + port.toUtf8() + "\n");
  }
  for (auto &port : output_ports_) {
    vcd_file_->write("b0 " + port.toUtf8() + "\n");
  }
  vcd_file_->write("$end\n");

  // simulation data
  int time = 0;
  if (!read_data_file_->open(QIODevice::ReadOnly | QIODevice::Text) ||
      !write_data_file_->open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Cannot open files to read waveform data.");
  }
  QString pre_read_data = "";
  QString pre_write_data = "";
  QString read_data = "";
  QString write_data = "";
  while (pre_read_data.size() < 64) {
    pre_read_data = "0" + pre_read_data;
    pre_write_data = "0" + pre_write_data;
  }
  while (!read_data_file_->atEnd() && !write_data_file_->atEnd()) {
    read_data = read_data_file_->readLine();
    write_data = write_data_file_->readLine();
    read_data.chop(1);
    write_data.chop(1);
    std::reverse(read_data.begin(), read_data.end());
    std::reverse(write_data.begin(), write_data.end());
    // qDebug() << "read_data" << read_data << " size" << read_data.size();
    // qDebug() << "write_data" << write_data << " size" << write_data.size();
    // compare the data with previous data
    if (read_data != pre_read_data || write_data != pre_write_data) {
      vcd_file_->write("#" + QString::number(time).toUtf8() + "\n");
    }
    if (read_data != pre_read_data) {
      for (auto i = 0; i < read_data.size(); ++i) {
        if ((read_data[i] != pre_read_data[i]) && output_ports_.contains(i)) {
          vcd_file_->write("b" + QString(read_data[i]).toUtf8() + " " +
                           output_ports_[i].toUtf8() + "\n");
        }
      }
      pre_read_data = read_data;
    }
    if (write_data != pre_write_data) {
      for (auto i = 0; i < write_data.size(); ++i) {
        if ((write_data[i] != pre_write_data[i]) && input_ports_.contains(i)) {
          vcd_file_->write("b" + QString(write_data[i]).toUtf8() + " " +
                           input_ports_[i].toUtf8() + "\n");
        }
      }
      pre_write_data = write_data;
    }
    time++;
  }
  read_data_file_->close();
  write_data_file_->close();
  vcd_file_->close();
}

VcdWorker::VcdWorker(WaveFormController *controller)
    : controller_(controller) {}

VcdWorker::~VcdWorker() {}

void VcdWorker::doWork() {
  QMutexLocker locker(&mutex_);
  controller_->writeToVcdFile();
}