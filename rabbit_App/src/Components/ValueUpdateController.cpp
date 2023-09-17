#include <QApplication>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <array>

#include "Components/Components.h"
#include "Components/ComponentsPanel.h"
#include "Components/ValueUpdateController.h"
#include "ThreadTimer.h"
#include "qnamespace.h"

using namespace rabbit_App::component;

ValueUpdateController::ValueUpdateController(ComponentsPanel *components_panel,
                                             QObject *parent)
    : QObject(parent), components_panel_(components_panel) {
  // update_timer_ = new ThreadTimer(this);
  // update_timer_->setInterval(1000 / 60);
  // // // connect(update_timer_, &ThreadTimer::timeOutSignal, this,
  // // //         &ValueUpdateController::process);
  // thread_worker_ = new ThreadWorker(new ProcessWorker(this), this);
  // connect(update_timer_, &ThreadTimer::timeOutSignal, thread_worker_,
  //         &ThreadWorker::work);
  time_thread_worker_ = new TimeThreadWorker(new ProcessWorker(this), this,
                                             Qt::BlockingQueuedConnection);
  time_thread_worker_->setInterval(1000 / 60);
}

ValueUpdateController::~ValueUpdateController() {}

void ValueUpdateController::onStartUpdate() {
  // update_timer_->start();
  read_queue_.clear();
  time_thread_worker_->start();
}

void ValueUpdateController::onStopUpdate() {
  // update_timer_->stop();
  time_thread_worker_->stop();
  read_queue_.clear();
}

void ValueUpdateController::onReceiveData(const std::vector<uint16_t> &data) {
  assert(data.size() % 4 == 0);

  for (size_t i = 0; i != data.size(); i += 4) {
    uint64_t value = 0;
    for (size_t j = 0; j != 4; ++j) {
      value |= (uint64_t(data[i + j]) << (j * 16));
    }
    // qDebug() << "Enqueue: " << value;
    read_queue_.enqueue(value);
  }
}

void ValueUpdateController::process() {
  // qDebug() << "ValueUpdateController::process() thread: "
  //          << QThread::currentThreadId();
  if (read_queue_.isEmpty()) {
    return;
  }
  for (auto component : components_panel_->components()) {
    component->rawComponent()->processReadData(read_queue_);
  }
  read_queue_.clear();
}

ProcessWorker::ProcessWorker(ValueUpdateController *controller)
    : controller_(controller) {}

ProcessWorker::~ProcessWorker() {}

void ProcessWorker::doWork() {
  // QMutexLocker locker(&mutex_);
  controller_->process();
}
