#include <QApplication>

#include "MainWindow.h"

using namespace rabbit_App;

int main(int argc, char **argv) {
  QApplication a(argc, argv);
  a.setStyle("fusion");
  MainWindow w;
  w.show();
  return a.exec();
}
