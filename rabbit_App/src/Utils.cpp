#include "Utils.h"
#include "qpainter.h"
#include "qpixmap.h"
#include <QPainter>

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