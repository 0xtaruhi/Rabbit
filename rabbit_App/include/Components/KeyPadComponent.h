#pragma once
#include "qlist.h"
#ifndef SMALL_KEYBOARD_COMPONENT_H
#define SMALL_KEYBOARD_COMPONENT_H

#include <QMap>
#include <QObject>
#include <QVector>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(KeyPad)
COMPONENT_CLASS_DECLARATION(SmallKeyPad)

/// @brief Key class
/// This class implements the key of the keypad.
class Key : public QWidget {
  Q_OBJECT

public:
  // constance
  static constexpr int kWidth = 40;
  static constexpr int kHeight = 40;
  static constexpr int kRoundRadius = 5;
  static constexpr int kBorderWidth = 2;
  static constexpr QColor kCharColor = QColor(255, 255, 255);
  static constexpr QColor kShadowColor = QColor(0, 0, 0, 100);
  static constexpr QColor kDefaultColor = QColor(108, 117, 125, 255);
  static constexpr QColor kHoverColor = QColor(90, 98, 104, 255);
  static constexpr QColor kPressedColor = QColor(84, 91, 98, 255);

public:
  Key(QWidget *parent = nullptr);
  Key(const QString &text, QWidget *parent = nullptr);
  virtual ~Key();

  bool isPressed() const { return is_pressed_; }

  /// @brief Set the text of the key.
  void setText(const QString &text);

  /// @brief Set whether the key is pressed.
  void setPressed(bool is_pressed);

  /// @brief Set whether the key is hovered.
  void setHovered(bool is_hovered);

  void setDefault();

protected:
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;

private:
  QString text_;
  bool is_pressed_ = false;
  bool is_hovered_ = false;

}; // class Key

/// @brief KeyPadRawComponent class
/// This class implements the 4x4 keypad component.
class KeyPadRawComponent : public AbstractRawComponent {
  Q_OBJECT

public:
  // constance
  static constexpr QColor kBackGroundColor = QColor(255, 255, 255);
  static constexpr QColor kBorderColor = QColor(90, 98, 104, 255);
  static constexpr int kBorderWidth = 3;
  static constexpr int kRoundRadius = 10;
  static constexpr int kShrink = 0;

public:
  KeyPadRawComponent(QWidget *parent = nullptr, bool is_4x4 = true);
  virtual ~KeyPadRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

  void updateAllKeys();

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;
  void initLayout();
  void setColumnNum(int column_num) { column_num_ = column_num; }
  void setRowNum(int row_num) { row_num_ = row_num; }
  void setKeyTexts(const QList<QString> &key_texts) { key_texts_ = key_texts; }
  void setKeyTexts(const QList<QString> &&key_texts) { key_texts_ = key_texts; }

private:
  /// @brief Map of the keys.
  QMap<QPair<int, int>, Key *> keys_map_;
  QList<QString> key_texts_;
  int column_num_;
  int row_num_;

}; // class KeyPadRawComponent

/// @brief SmallKeyPadRawComponent class
/// This class implements the 3x4 keypad component.
/// This class is inherited from KeyPadRawComponent class.
class SmallKeyPadRawComponent : public KeyPadRawComponent {
  Q_OBJECT

public:
  SmallKeyPadRawComponent(QWidget *parent = nullptr);
  virtual ~SmallKeyPadRawComponent();

}; // class SmallKeyPadRawComponent

} // namespace rabbit_App::component

#endif // SMALL_KEYBOARD_COMPONENT_H