#ifndef PANEL_GUI_UPDATE_CONTROLLER_H
#define PANEL_GUI_UPDATE_CONTROLLER_H

#include <QObject>
#include <QRandomGenerator>
#include <QThread>
#include <QTimer>

namespace rabbit_App::component {

class PanelGuiUpdateController : public QObject {
  Q_OBJECT

public:
  PanelGuiUpdateController(QObject *parent = nullptr);
  ~PanelGuiUpdateController();

public slots:
  void onChangeFreshFrequency(int fresh_freq);
  void onStartUpdate();
  void onStopUpdate();
  void onUpdatePanel();

signals:
  void updateGui();

private:
  QTimer *update_timer_;
  int fresh_freq_;

  static constexpr auto kDefaultFreshFrequency = 60;
};

} // namespace rabbit_App::component

#endif // PANEL_GUI_UPDATE_CONTROLLER_H
