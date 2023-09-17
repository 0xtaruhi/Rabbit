#pragma once
#ifndef TEXT_LCD_COMPONENT_H
#define TEXT_LCD_COMPONENT_H

#include <QObject>
#include <QVector>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"

namespace rabbit_App::component {

COMPONENT_CLASS_DECLARATION(TextLCD)

class CharBlock final : public QWidget {
  Q_OBJECT

public:
  CharBlock(QWidget *parent = nullptr);
  virtual ~CharBlock();

  /// @brief Set the character code. The code is the assic code of the character
  /// minus 0x20.
  void setCharCode(const quint8 &char_code);

protected:
  void paintEvent(QPaintEvent *event) override;

private:
  quint8 char_code_ = 0x00;
};

class TextLCDRawComponent final : public AbstractRawComponent {
  Q_OBJECT

public:
  TextLCDRawComponent(QWidget *parent = nullptr);
  virtual ~TextLCDRawComponent();

  void reset() override;

  void processReadData(QQueue<uint64_t> &read_queue) override;
  uint64_t getWriteData() const override;

  /// @brief Set the character at the given position.
  /// @param char_code The character code to set.
  /// @param char_pos The position of the character to set.
  void setChar(const quint8 &char_code, const quint8 &char_pos);

  /// @brief Set the character at the current cursor position. The
  /// cursor will be moved to the next position.
  /// @param char_code The character code to set.
  void setChar(const quint8 &char_code);

  /// @brief Return current cursor position.
  /// @return Current cursor position.
  auto cursorPos() const noexcept { return cur_pos_; }

  /// @brief Move the cursor to the next position. When the cursor is at
  /// the end of the line, it will be moved to the beginning of the next.
  /// When the cursor is at the end of the last line, it will be moved to
  /// the beginning of the first line.
  void moveCursorNext();

  /// @brief Set the cursor position.
  /// @param pos The position to set.
  void setCursorPos(const int &pos);

  /// @brief Move the cursor to the beginning of the first line.
  void moveCursorHome();

protected:
  void paintEvent(QPaintEvent *event) override;
  void initPorts() override;

private:
  QVector<CharBlock *> char_blocks_;
  int cur_pos_ = 0;
};

} // namespace rabbit_App::component

#endif // TEXT_LCD_COMPONENT_H
