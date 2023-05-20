#include "MainWindow.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/StyleTools.h"
#include "TabToolbar/TabToolbar.h"

using namespace rabbit_App;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  initMembers();
  initLayout();
}

MainWindow::~MainWindow() {}

void MainWindow::initMembers() {
  ttb_ = new tt::Builder(this);
  ttb_->SetCustomWidgetCreator("pushButton",
                               []() { return new QPushButton(); });
  tt::TabToolbar *tabToolbar =
      ttb_->CreateTabToolbar(":/tt/tabtoolbar/tabtoolbar.json");

  tt::Group *stylesGroup = (tt::Group *)((*ttb_)["Styles"]);
  stylesGroup->AddSeparator();
  QStringList styles = tt::GetRegisteredStyles();
  for (int i = 0; i < styles.size(); i++) {
    const QString &styleName = styles.at(i);
    QPushButton *btn = new QPushButton(styles[i]);
    QObject::connect(btn, &QPushButton::clicked, [&styleName, tabToolbar]() {
      tabToolbar->SetStyle(styleName);
    });
    stylesGroup->AddWidget(btn);
  }

  addToolBar(Qt::TopToolBarArea, tabToolbar);
}

void MainWindow::initLayout() {}

void MainWindow::initConnections() {}
