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

/// @brief ValueUpdateController class
/// This class is used to update the value of the components.
/// It processes the read data in a worker thread.
/// The frequency of the update is 60Hz.
class ValueUpdateController final : public QObject {

  Q_OBJECT

  friend class ProcessWorker;

public:
  ValueUpdateController(ComponentsPanel *components_panel,
                        QObject *parent = nullptr);
  virtual ~ValueUpdateController();

public slots:
  /// @brief Start the update timer.
  void onStartUpdate();

  /// @brief Stop the update timer.
  void onStopUpdate();

  /// @brief Set the read queue.
  void onReceiveData(const std::vector<uint16_t> &data);

private slots:
  /// @brief Process the read queue. Called by the worker.
  void process();

private:
  ComponentsPanel *components_panel_;
  // ThreadTimer *update_timer_;
  // ThreadWorker *process_worker_;
  // ThreadWorker *thread_worker_;

  /// @brief The worker manager
  TimeThreadWorker *time_thread_worker_;

  /// @brief The read queue
  /// The read queue is used to store the read data.
  /// It may be very long, but every data should be processed.
  QQueue<uint64_t> read_queue_;
};

/// @brief ProcessWorker class
/// This class is used to process the read queue in ValueUpdateController class.
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
