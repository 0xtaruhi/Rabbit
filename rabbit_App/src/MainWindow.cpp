#include "MainWindow.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/StyleTools.h"
#include "TabToolbar/TabToolbar.h"

using namespace rabbit_App;

constexpr auto kMinWindowWidth = 800;
constexpr auto kMinWindowHeight = 600;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initMembers();
  initLayout();
}

MainWindow::~MainWindow() {}

void MainWindow::initMembers() {
  builder_ = new tt::Builder(this);
  builder_->SetCustomWidgetCreator("pushButton",
                                   []() { return new QPushButton(); });
  tab_tool_bar_ = builder_->CreateTabToolbar(":/tt/tabtoolbar/tabtoolbar.json");
}

void MainWindow::initLayout() {
  setMinimumSize(kMinWindowWidth, kMinWindowHeight);
  setWindowTitle("Rabbit");

  // Init TabToolbar
  tt::Group *styles_group = (tt::Group *)((*builder_)["Styles"]);
  styles_group->AddSeparator();
  QStringList styles = tt::GetRegisteredStyles();
  for (int i = 0; i < styles.size(); i++) {
    const QString &style_name = styles.at(i);
    QPushButton *btn = new QPushButton(styles[i]);
    QObject::connect(btn, &QPushButton::clicked, [=]() {
      tab_tool_bar_->SetStyle(style_name);
    });
    styles_group->AddWidget(btn);
  }
  addToolBar(Qt::TopToolBarArea, tab_tool_bar_);
}

void MainWindow::initConnections() {}
