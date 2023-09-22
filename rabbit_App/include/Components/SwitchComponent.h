#pragma once
#ifndef SWITCH_COMPONENT_H
#define SWITCH_COMPONENT_H

#include <QPixmap>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(Switch)

/// @brief SwitchRawComponent class
/// This class implements the Switch component.
class SwitchRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  SwitchRawComponent(QWidget *parent = nullptr);
  virtual ~SwitchRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

  void initPorts() override;
  void initConnections();

  const QPixmap *switch_picture_;
  QLabel *switch_picture_label_;

  static const QPixmap &switchOnPicture() {
    static const QPixmap switch_on_picture_ =
        QPixmap(":/icons/icons/icons8-switch-on-94.png");
    return switch_on_picture_;
  };

  static const QPixmap &switchOffPicture() {
    static const QPixmap switch_off_picture_ =
        QPixmap(":/icons/icons/icons8-switch-off-94.png");
    return switch_off_picture_;
  }

signals:
  void switchToggled();

protected slots:
  void onSwitchToggled();

private:
  bool is_on_ = false;
};

} // namespace rabbit_App::component

#endif // SWITCH_COMPONENT_H
