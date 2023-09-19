#pragma once
#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include <QCheckBox>
#include <QLabel>
#include <QPixmap>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(Button)

class ButtonRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  ButtonRawComponent(QWidget *parent = nullptr);
  virtual ~ButtonRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;

  void initPorts() override;
  void initConnections();

  const QPixmap *button_picture_;
  QLabel *button_picture_label_;

  static const QPixmap &buttonPushPicture() {
    static const QPixmap button_push_picture_ =
        QPixmap(":/icons/icons/icons8-button-push-94.png");
    button_push_picture_.scaled(94, 94, Qt::KeepAspectRatio);
    return button_push_picture_;
  };

  static const QPixmap &buttonReleasePicture() {
    static const QPixmap button_release_picture_ =
        QPixmap(":/icons/icons/icons8-button-release-94.png");
    button_release_picture_.scaled(94, 94, Qt::KeepAspectRatio);
    return button_release_picture_;
  }

signals:
  void buttonPush();
  void buttonRelease();

protected slots:
  void onButtonPush();
  void onButtonRelease();

private:
  bool is_pushed_ = false;

}; // class ButtonRawComponent

} // namespace rabbit_App::component

#endif // BUTTON_COMPONENT_H