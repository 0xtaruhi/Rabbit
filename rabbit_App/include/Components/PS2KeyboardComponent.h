#pragma once
#ifndef PS2KEYBOARD_COMPONENT_H
#define PS2KEYBOARD_COMPONENT_H

#include <QLabel>
#include <QRadioButton>
#include <queue>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(PS2Keyboard)

/// @brief PS2KeyboardRawComponent class
/// This class implements the PS2Keyboard component.
class PS2KeyboardRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  PS2KeyboardRawComponent(QWidget *parent = nullptr);
  virtual ~PS2KeyboardRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;
  // special functions

  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

  void initConnections();

  const QPixmap *keyboard_picture_;
  QLabel *keyboard_picture_label_;

  static const QPixmap &keyboardPictureStatic() {
    static const QPixmap kb_picture_ =
        QPixmap(":/icons/icons/icons8-keyboard-94.png");
    return kb_picture_;
  };

  static const QPixmap &keyboardPictureInUse() {
    static const QPixmap kb_inuse_picture_ =
        QPixmap(":/icons/icons/icons8-keyboard-inuse-94.png");
    return kb_inuse_picture_;
  };

  auto keyMapIndex(const int &keyType) -> int;

  bool checkSpecialKey2(int keyValue);
  void pclkEnqueue(int cycle, bool data);
  void pdataEnqueue(int cycle, bool data);
  void pclkWriteDataGenerate(int keyValue);
  void pdataWriteDataGenerate(int keyValue);

signals:
  void keyPressed(int keyType, int state);
  void keyReleased(int keyType, int state);

protected slots:
  void keyProcess(int keyType, int state);

private:
  int key_pressed_num_ = 0;
  mutable std::queue<bool> pclk_to_write_;
  mutable std::queue<bool> pdata_to_write_;
};

} // namespace rabbit_App::component

#endif // PS2KEYBOARD_COMPONENT_H