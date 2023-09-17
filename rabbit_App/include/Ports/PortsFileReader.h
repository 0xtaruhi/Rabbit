#pragma once
#ifndef PORTS_FILE_READER
#define PORTS_FILE_READER

#include <QHash>
#include <QVector>


#include <QObject>

#include "Ports/Port.h"

namespace rabbit_App::ports {

class PortsFileReader : public QObject {
  Q_OBJECT

public:
  PortsFileReader(QObject *parent = nullptr);
  ~PortsFileReader();

  void readFromFile(const QString &file_path);

  const auto &inputs() const noexcept { return inputs_; }
  const auto &outputs() const noexcept { return outputs_; }
  const Port &findPort(const QString &port_name, PortType type) const;
  const Port &clock() const noexcept { return clock_; }

private:
  QVector<Port> inputs_;
  QVector<Port> outputs_;
  QHash<QString, int> inputs_order_;
  QHash<QString, int> outputs_order_;
  Port clock_;
};

} // namespace rabbit_App::ports

#endif // PORTS_FILE_READER
