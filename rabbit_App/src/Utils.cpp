#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QPainter>
#include <QPixmap>

#include "Utils.h"

using namespace rabbit_App;

QPixmap rabbit_App::getTransparentPixmap(const QPixmap &pixmap, ushort alpha) {
  auto transparent_pixmap(pixmap);
  QPainter painter(&transparent_pixmap);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.fillRect(transparent_pixmap.rect(),
                   QColor(0, 0, 0, alpha)); // semi-transparent

  return transparent_pixmap;
}

void rabbit_App::setTransparentPixmap(QPixmap &pixmap, ushort alpha) {
  QPainter painter(&pixmap);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
  painter.fillRect(pixmap.rect(), QColor(0, 0, 0, alpha)); // semi-transparent
}

const QString &rabbit_App::getRelativePath(const QString &absulute_path,
                                           const QString &base_path) {
  // qDebug() << "getRelativePath";
  // qDebug() << "absulute_path: " << absulute_path << "base_path: " <<
  // base_path;
  static QString relative_path;
  QDir base_dir(base_path);
  base_dir.cdUp();
  relative_path = base_dir.relativeFilePath(absulute_path);
  // qDebug() << "relative_path: " << relative_path;
  return relative_path;
}

const QString &rabbit_App::getAbsolutePath(const QString &relative_path,
                                           const QString &base_path) {
  // qDebug() << "getAbsolutePath";
  // qDebug() << "relative_path: " << relative_path << "base_path: " <<
  // base_path;
  static QString absolute_path;
  // get the directory of the base_path
  QDir base_dir(base_path);
  base_dir.cdUp();
  QDir path(base_dir.absoluteFilePath(relative_path));
  // qDebug() << "absolute_path: " << absolute_path;
  absolute_path = path.absolutePath();
  return absolute_path;
}