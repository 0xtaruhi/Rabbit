#pragma once
#include "qpushbutton.h"
#include "qtmetamacros.h"
#ifndef COMPONENT_TITLE_BAR_H
#define COMPONENT_TITLE_BAR_H

#include <QLabel>
#include <QWidget>

namespace rabbit_App::component {

class ComponentTitleBar : public QWidget {
  Q_OBJECT

public:
  ComponentTitleBar(QWidget *parent = nullptr,
                    const QString &title = "Untitled");
  virtual ~ComponentTitleBar();

  QSize sizeHint() const override;

  QString title() const noexcept { return title_; }
  void setTitle(const QString &title);

protected:
  void paintEvent(QPaintEvent *event) override;

  void initMembers();
  void initLayout();

private slots:
  void onDeleteBtnClicked();
  void onSettingsBtnClicked();

signals:
  void deleteBtnClicked();
  void settingsBtnClicked();

private:
  QLabel *title_label_;
  QString title_;

  QPushButton *settings_btn_;
  QPushButton *delete_btn_;
};

} // namespace rabbit_App::component

#endif // COMPONENT_TITLE_BAR_H
