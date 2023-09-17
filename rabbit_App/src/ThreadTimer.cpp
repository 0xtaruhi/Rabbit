#include "ThreadTimer.h"
#include "qnamespace.h"
#include "qtimer.h"

#include <QDebug>
#include <QThread>

using namespace rabbit_App;

// ThreadTimer::ThreadTimer(QObject *parent) : QObject(parent) {
//   thread_ = new QThread(this);
//   is_outside_thread_ = false;
//   timer_ = new QTimer();
//   timer_->setInterval(kDefaultInterval);
//   timer_->moveToThread(thread_);
//   connect(timer_, &QTimer::timeout, this, &ThreadTimer::onTimerTimeout,
//           Qt::DirectConnection);
//   connect(thread_, &QThread::started, timer_,
//   QOverload<>::of(&QTimer::start)); connect(thread_, &QThread::finished,
//   timer_, &QTimer::stop);
// }

// ThreadTimer::~ThreadTimer() {
//   if (timer_->isActive()) {
//     timer_->stop();
//   }
//   timer_->deleteLater();
//   if (!is_outside_thread_) {
//     if (thread_->isRunning()) {
//       thread_->quit();
//     }
//     thread_->wait();
//     thread_->deleteLater();
//   }
// }

// void ThreadTimer::start() {
//   timer_->setInterval(interval_);
//   thread_->start();
// }

// void ThreadTimer::start(int interval) {
//   interval_ = interval;
//   timer_->setInterval(interval_);
//   thread_->start();
// }

// void ThreadTimer::stop() {
//   thread_->quit();
//   thread_->wait();
// }

// void ThreadTimer::onTimerTimeout() {
//   // qDebug() << "ThreadTimer::onTimerTimeout() thread: " <<
//   // QThread::currentThreadId();
//   emit timeOutSignal();
// }

// SingleThreadTimer::SingleThreadTimer() {
//   timer_count_ = 0;
//   thread_ = new QThread(this);
//   is_outside_thread_ = false;
//   this->moveToThread(thread_);
//   thread_->start();
// }

// SingleThreadTimer::SingleThreadTimer(QThread *thread) {
//   timer_count_ = 0;
//   thread_ = thread;
//   is_outside_thread_ = true;
//   this->moveToThread(thread_);
// }

// SingleThreadTimer::~SingleThreadTimer() {
//   if (!is_outside_thread_) {
//     if (thread_->isRunning()) {
//       thread_->quit();
//     }
//     thread_->wait();
//     thread_->deleteLater();
//   }
// }

// void SingleThreadTimer::onTimerTimeout() {
//   // qDebug() << "SingleThreadTimer::onTimerTimeout() thread: " <<
//   // QThread::currentThreadId();
//   timer_count_--;
//   if (timer_count_ > 0) {
//     return;
//   } else if (timer_count_ == 0) {
//     emit timeOutSignal();
//   }
// }

// void SingleThreadTimer::start() {
//   timer_count_++;
//   QTimer::singleShot(interval_, this, &SingleThreadTimer::onTimerTimeout);
// }

// void SingleThreadTimer::start(int interval) {
//   timer_count_++;
//   QTimer::singleShot(interval, this, &SingleThreadTimer::onTimerTimeout);
// }

ThreadWorker::ThreadWorker(QObject *parent) : QObject(parent) {
  thread_ = new QThread(this);
}

ThreadWorker::ThreadWorker(Worker *worker, QObject *parent,
                           Qt::ConnectionType type)
    : QObject(parent) {
  thread_ = new QThread(this);
  setWorker(worker, type);
}

ThreadWorker::~ThreadWorker() {
  if (thread_->isRunning()) {
    thread_->quit();
  }
  thread_->wait();
  if (is_worker_set_) {
    emit deleteWorker();
  }
}

void ThreadWorker::setWorker(Worker *worker, Qt::ConnectionType type) {
  worker_ = worker;

  worker_->moveToThread(thread_);
  // connect(thread_, &QThread::finished, worker_, &QObject::deleteLater,
  // Qt::DirectConnection);
  connect(this, &ThreadWorker::deleteWorker, worker_, &QObject::deleteLater,
          Qt::QueuedConnection);
  connect(this, &ThreadWorker::operate, worker_, &Worker::doWork, type);
  connect(worker_, &Worker::resultReady, this, &ThreadWorker::handleResults,
          Qt::QueuedConnection);
  is_worker_set_ = true;
  // thread_->start();
}

void ThreadWorker::work() { emit operate(); }

void ThreadWorker::start() { thread_->start(); }

void ThreadWorker::stop() {
  thread_->quit();
  thread_->wait();
}

TimeThreadWorker::TimeThreadWorker(QObject *parent) : ThreadWorker(parent) {
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &TimeThreadWorker::work);
}

TimeThreadWorker::TimeThreadWorker(Worker *worker, QObject *parent,
                                   Qt::ConnectionType type)
    : ThreadWorker(worker, parent, type) {
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &TimeThreadWorker::work);
}

TimeThreadWorker::~TimeThreadWorker() {
  if (timer_->isActive()) {
    timer_->stop();
  }
}

void TimeThreadWorker::start() {
  thread_->start();
  timer_->start();
}

void TimeThreadWorker::start(int interval) {
  thread_->start();
  timer_->start(interval);
}

void TimeThreadWorker::stop() {
  timer_->stop();
  thread_->quit();
  thread_->wait();
}
