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

/// @brief ButtonRawComponent class implements button component.
///        It is used to send a signal when the button is pressed.
///        Inherited from AbstractRawComponent.
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

  /// @brief Mouse move event.
  ///        Override this function to avoid moving the component
  ///        otherwise it will be often moved when the mouse is pressed.
  void mouseMoveEvent(QMouseEvent *event) override;

  void initPorts() override;

  /// @brief Initialize connections.
  void initConnections();

  /// @brief Button picture to be displayed.
  const QPixmap *button_picture_;
  /// @brief Label to display the button picture.
  QLabel *button_picture_label_;

  /// @brief Button push picture.
  /// This picture is used when the button is pushed. It is static.
  static const QPixmap &buttonPushPicture() {
    static const QPixmap button_push_picture_ =
        QPixmap(":/icons/icons/icons8-button-push-94.png");
    button_push_picture_.scaled(94, 94, Qt::KeepAspectRatio);
    return button_push_picture_;
  };

  /// @brief Button release picture.
  /// This picture is used when the button is released. It is static.
  static const QPixmap &buttonReleasePicture() {
    static const QPixmap button_release_picture_ =
        QPixmap(":/icons/icons/icons8-button-release-94.png");
    button_release_picture_.scaled(94, 94, Qt::KeepAspectRatio);
    return button_release_picture_;
  }

signals:
  /// @brief Signal is emitted when the button is pushed.
  void buttonPush();
  /// @brief Signal is emitted when the button is released.
  void buttonRelease();

protected slots:
  /// @brief Slot is called when the button is pushed.
  void onButtonPush();
  /// @brief Slot is called when the button is released.
  void onButtonRelease();

private:
  /// @brief Whether the button is pushed.
  bool is_pushed_ = false;

}; // class ButtonRawComponent

} // namespace rabbit_App::component

#endif // BUTTON_COMPONENT_H