#include "MainWindow.h"
#include "TabToolbar/Group.h"
#include "TabToolbar/StyleTools.h"
#include "TabToolbar/TabToolbar.h"

using namespace rabbit_App;

constexpr auto kMinWindowWidth = 800;
constexpr auto kMinWindowHeight = 600;
constexpr auto kDefaultStyle = "Kool";

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

  tab_tool_bar_->SetStyle("Kool");
  addToolBar(Qt::TopToolBarArea, tab_tool_bar_);
}

void MainWindow::initConnections() {}
