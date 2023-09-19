#include <QApplication>
#include <QDateTime>
#include <QThread>
#include <QTimer>

#include "Components/PanelGuiUpdateController.h"

using namespace rabbit_App::component;

PanelGuiUpdateController::PanelGuiUpdateController(QObject *parent)
    : QObject(parent) {
  update_timer_ = new QTimer(this);
  fresh_freq_ = kDefaultFreshFrequency;
  update_timer_->setInterval(1000 / fresh_freq_);
  connect(update_timer_, &QTimer::timeout, this,
          &PanelGuiUpdateController::onUpdatePanel);
}

PanelGuiUpdateController::~PanelGuiUpdateController() {}

void PanelGuiUpdateController::onStartUpdate() { update_timer_->start(); }

void PanelGuiUpdateController::onStopUpdate() { update_timer_->stop(); }

void PanelGuiUpdateController::onChangeFreshFrequency(int fresh_freq) {
  fresh_freq_ = fresh_freq;
  update_timer_->setInterval(1000 / fresh_freq);
}

void PanelGuiUpdateController::onUpdatePanel() { emit updateGui(); }
