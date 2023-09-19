#ifndef VALUE_UPDATE_CONTROLLER_H
#define VALUE_UPDATE_CONTROLLER_H

#include <QObject>
#include <QQueue>
#include <QThread>

#include "Components/ValueUpdateController.h"
#include "ThreadTimer.h"

namespace rabbit_App::component {

class ComponentsPanel;
class ProcessWorker;
class ValueUpdateController final : public QObject {

  Q_OBJECT

  friend class ProcessWorker;

public:
  ValueUpdateController(ComponentsPanel *components_panel,
                        QObject *parent = nullptr);
  virtual ~ValueUpdateController();

public slots:
  void onStartUpdate();
  void onStopUpdate();
  void onReceiveData(const std::vector<uint16_t> &data);

private slots:
  void process();

private:
  ComponentsPanel *components_panel_;
  // ThreadTimer *update_timer_;
  // ThreadWorker *process_worker_;
  // ThreadWorker *thread_worker_;
  TimeThreadWorker *time_thread_worker_;
  QQueue<uint64_t> read_queue_;
};

class ProcessWorker : public Worker {
  Q_OBJECT

public:
  ProcessWorker(ValueUpdateController *controller);
  ~ProcessWorker();

protected:
  void doWork() override;

private:
  ValueUpdateController *controller_;

}; // class ProcessWorker

} // namespace rabbit_App::component

#endif // VALUE_UPDATE_CONTROLLER_H
