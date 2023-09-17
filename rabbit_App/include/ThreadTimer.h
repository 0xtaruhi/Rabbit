#ifndef THREAD_TIMER_H
#define THREAD_TIMER_H

#include "qnamespace.h"
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QTimer>
#include <QVariant>

namespace rabbit_App {

// class ThreadTimer : public QObject {
//   Q_OBJECT

// public:
//   ThreadTimer(QObject *parent = nullptr);
//   virtual ~ThreadTimer();

//   void setInterval(int interval) { interval_ = interval; }
//   void setThread(QThread *thread) {
//     thread_ = thread;
//     is_outside_thread_ = true;
//   }

//   void start();
//   void start(int interval);
//   void stop();

//   void onTimerTimeout();

// signals:
//   void timeOutSignal();

// private:
//   QTimer *timer_;
//   QThread *thread_;
//   uint interval_ = kDefaultInterval;

//   static constexpr uint kDefaultInterval = 100;

//   bool is_outside_thread_;

// }; // class ThreadTimer

// class SingleThreadTimer : public QObject {
//   Q_OBJECT

// public:
//   SingleThreadTimer();
//   SingleThreadTimer(QThread *thread);
//   virtual ~SingleThreadTimer();

//   void setInterval(int interval) { interval_ = interval; }
//   void setThread(QThread *thread) {
//     thread_ = thread;
//     is_outside_thread_ = true;
//   }

//   void start();
//   void start(int interval);

//   void onTimerTimeout();

// signals:
//   void timeOutSignal();

// private:
//   uint timer_count_ = 0;
//   QThread *thread_;
//   uint interval_ = kDefaultInterval;
//   static constexpr uint kDefaultInterval = 100;

//   bool is_outside_thread_;

// }; // class SingleThreadTimer

class Worker : public QObject {
  Q_OBJECT

public:
  Worker() {}
  ~Worker() {}

public slots:
  virtual void doWork() = 0;

signals:
  void resultReady(const QVariant &result);

protected:
  QMutex mutex_;

}; // class Worker

class ThreadWorker : public QObject {
  Q_OBJECT

public:
  ThreadWorker(QObject *parent = nullptr);
  ThreadWorker(Worker *worker, QObject *parent = nullptr, Qt::ConnectionType type = Qt::QueuedConnection);
  ~ThreadWorker();

  virtual void setWorker(Worker *worker, Qt::ConnectionType type = Qt::QueuedConnection);
  void work();

  virtual void start();
  virtual void stop();

public slots:
  virtual void handleResults(const QVariant &) {}
signals:
  void operate();
  void deleteWorker();

protected:
  QThread *thread_;
  Worker *worker_;

  bool is_worker_set_ = false;

}; // class ThreadWorker

class TimeThreadWorker : public ThreadWorker {
  Q_OBJECT

public:
  TimeThreadWorker(QObject *parent = nullptr);
  TimeThreadWorker(Worker *worker, QObject *parent = nullptr, Qt::ConnectionType type = Qt::QueuedConnection);
  ~TimeThreadWorker();

  void setInterval(int interval) { timer_->setInterval(interval); }

  virtual void start() override;
  void start(int interval);
  virtual void stop() override;

private:
  QTimer* timer_;

}; // class TimeThreadWorker

} // namespace rabbit_App

#endif // THREAD_TIMER_H