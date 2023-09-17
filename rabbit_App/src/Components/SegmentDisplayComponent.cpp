#include <QGradient>
#include <QLayout>
#include <QPainter>
#include <QRadialGradient>
#include <QSize>
#include <QThreadPool>

#include "Components/AbstractComponent.h"
#include "Components/ComponentMacro.h"
#include "Components/ComponentSettingsDialog.h"
#include "Components/SegmentDisplayComponent.h"
#include "qbrush.h"

using namespace rabbit_App::component;

const QList<QPoint> SingleSegmentDisplay::a_points = {
    QPoint(22, 9), QPoint(29, 3),  QPoint(72, 3),
    QPoint(79, 9), QPoint(69, 17), QPoint(29, 17)};
const QList<QPoint> SingleSegmentDisplay::b_points = {
    {81, 12}, {87, 19}, {84, 56}, {75, 66}, {68, 59}, {72, 20}};
const QList<QPoint> SingleSegmentDisplay::c_points = {
    {75, 70}, {82, 77}, {79, 115}, {71, 124}, {63, 115}, {67, 78}};
const QList<QPoint> SingleSegmentDisplay::d_points = {
    {60, 117}, {67, 126}, {61, 133}, {16, 133}, {10, 126}, {18, 118}};
const QList<QPoint> SingleSegmentDisplay::e_points = {
    {6, 124}, {0, 118}, {5, 77}, {13, 70}, {21, 78}, {16, 115}};
const QList<QPoint> SingleSegmentDisplay::f_points = {
    {15, 65}, {8, 57}, {11, 19}, {19, 11}, {27, 20}, {23, 58}};
const QList<QPoint> SingleSegmentDisplay::g_points = {
    {17, 68}, {26, 61}, {65, 60}, {72, 68}, {64, 76}, {25, 76}};

SingleSegmentDisplay::SingleSegmentDisplay(QWidget *parent) : QWidget(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  segments_.resize(8);
  segments_.fill(false);
  en_ = false;

  brushColor = QColor(255, 0, 0, 255);
  penColor = QColor(193, 0, 0, 255);
  brushColorOff = QColor(60, 60, 60, 255);
  penColorOff = QColor(100, 100, 100, 255);
  penWidth = 3;
}

SingleSegmentDisplay::~SingleSegmentDisplay() {}

void SingleSegmentDisplay::setSegment(const int segment_section,
                                      const bool value) {
  if (!value && segments_[segment_section] && vision_persistence_ != 0) {
    on_persistence_[segment_section] = true;
    counter_[segment_section] = 0;
  }
  segments_[segment_section] = value;
}

void SingleSegmentDisplay::setEn(const bool en) {
  en_ = en;
  for (auto i = 0; i < kSegmentCount; i++) {
    setSegment(i, false);
  }
}

void SingleSegmentDisplay::reset() {
  for (auto i = 0; i < kSegmentCount; i++) {
    segments_[i] = false;
    on_persistence_[i] = false;
    counter_[i] = 0;
  }
  en_ = false;
  // update();
}

void SingleSegmentDisplay::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setWindow(0, 0, 94, 140);
  painter.setRenderHint(QPainter::Antialiasing, true);
  paintOneSegment(painter, a_points, SingleSegmentDisplay::A);
  paintOneSegment(painter, b_points, SingleSegmentDisplay::B);
  paintOneSegment(painter, c_points, SingleSegmentDisplay::C);
  paintOneSegment(painter, d_points, SingleSegmentDisplay::D);
  paintOneSegment(painter, e_points, SingleSegmentDisplay::E);
  paintOneSegment(painter, f_points, SingleSegmentDisplay::F);
  paintOneSegment(painter, g_points, SingleSegmentDisplay::G);
  paintDP(painter);
}

void SingleSegmentDisplay::paintOneSegment(QPainter &painter,
                                           const QList<QPoint> &points,
                                           const int segment) {
  if (on_persistence_[segment]) {
    counter_[segment]++;
    auto max = vision_persistence_ * 60 / 3000;
    if (counter_[segment] > max) {
      on_persistence_[segment] = false;
      counter_[segment] = 0;
    }
  }
  auto is_on = (segments_[segment] && en_) || on_persistence_[segment];
  auto brush_color = is_on ? this->brushColor : this->brushColorOff;
  auto pen_color = is_on ? this->penColor : this->penColorOff;
  painter.setBrush(QBrush(brush_color, Qt::SolidPattern));
  painter.setPen(QPen(pen_color, penWidth, Qt::SolidLine, Qt::RoundCap));
  painter.drawPolygon(points.data(), points.size());
}

void SingleSegmentDisplay::paintDP(QPainter &painter) {
  auto is_on = segments_[SingleSegmentDisplay::DP] && en_;
  auto brush_color = is_on ? this->brushColor : this->brushColorOff;
  auto pen_color = is_on ? this->penColor : this->penColorOff;
  painter.setBrush(QBrush(brush_color, Qt::SolidPattern));
  painter.setPen(QPen(pen_color, penWidth, Qt::SolidLine, Qt::RoundCap));
  painter.drawEllipse(QPoint(83, 130), 7, 7);
}

SegmentDisplayRawComponent::SegmentDisplayRawComponent(QWidget *parent)
    : AbstractRawComponent(parent) {
  is_low_active_ = true;
  single_segment_display_ = new SingleSegmentDisplay(this);
  auto layout = new QHBoxLayout(this);
  layout->addWidget(single_segment_display_);
  initPorts();
}

SegmentDisplayRawComponent::~SegmentDisplayRawComponent() {}

void SegmentDisplayRawComponent::reset() { single_segment_display_->reset(); }

void SegmentDisplayRawComponent::processReadData(QQueue<uint64_t> &read_queue) {
  for (auto data : read_queue) {
    single_segment_display_->setEn(true);
    auto &segments = single_segment_display_->segments();
    for (auto i = SingleSegmentDisplay::A; i <= SingleSegmentDisplay::DP; ++i) {
      segments[i] =
          ((data >> (output_ports_[i].pin_index - 1)) & 0x1) ^ is_low_active_;
    }
  }
}

uint64_t SegmentDisplayRawComponent::getWriteData() const { return 0; }

void SegmentDisplayRawComponent::paintEvent(QPaintEvent *event) {}

void SegmentDisplayRawComponent::initPorts() {
  // ports_index_.resize(8);
  for (int i = 'A'; i <= 'G'; i++) {
    appendPort(output_ports_, QString(static_cast<char>(i)),
               ports::PortType::Output);
  }
  appendPort(output_ports_, "DP", ports::PortType::Output);
}

FourDigitSegmentDisplayRawComponent::FourDigitSegmentDisplayRawComponent(
    QWidget *parent)
    : AbstractRawComponent(parent) {
  is_low_active_ = true;
  auto layout = new QHBoxLayout(this);
  for (auto i = 0; i < kDigitCount; i++) {
    multi_segments_.append(new SingleSegmentDisplay(this));
    segment_select_.append(false);
    layout->addWidget(multi_segments_.back());
  }
  initPorts();
}

FourDigitSegmentDisplayRawComponent::~FourDigitSegmentDisplayRawComponent() {
  segment_select_.clear();
}

void FourDigitSegmentDisplayRawComponent::reset() {
  for (auto &single_segment : multi_segments_) {
    single_segment->reset();
  }
  segment_select_.fill(false);
  update();
}

void FourDigitSegmentDisplayRawComponent::processReadData(
    QQueue<uint64_t> &read_queue) {
  for (auto data : read_queue) {
    auto index = -1;
    for (auto i = 0; i < kDigitCount; i++) {
      // qDebug() << "In seg : data : " << data << " pin_index: " <<
      // output_ports_[i].pin_index <<
      //           " data_section: " << ((data >> (output_ports_[i].pin_index -
      //           1)) & 0x1);
      segment_select_[i] =
          ((data >> (output_ports_[i].pin_index - 1)) & 0x1) ^ is_low_active_;
      if (segment_select_[i] && index == -1) {
        index = i;
      }
    }
    if (index == -1) {
      reset();
    } else {
      for (auto i = 0; i < kDigitCount; i++) {
        multi_segments_[i]->setEn(false);
      }
      for (auto i = 0; i < SingleSegmentDisplay::kSegmentCount; i++) {
        multi_segments_[index]->setEn(true);
        multi_segments_[index]->setSegment(
            i,
            ((data >> (output_ports_[i + kDigitCount].pin_index - 1)) & 0x1) ^
                is_low_active_);
      }
    }
  }
}

void FourDigitSegmentDisplayRawComponent::paintEvent(QPaintEvent *event) {}

uint64_t FourDigitSegmentDisplayRawComponent::getWriteData() const { return 0; }

void FourDigitSegmentDisplayRawComponent::setVisionPersistence(
    int vision_persistence) {
  vision_persistence_ = vision_persistence;
  for (auto single_segment : multi_segments_) {
    single_segment->setVisionPersistence(vision_persistence);
  }
}

void FourDigitSegmentDisplayRawComponent::initPorts() {
  for (auto i = 0; i < kDigitCount; i++) {
    appendPort(output_ports_, QString("SEL%1").arg(i), ports::PortType::Output);
  }
  for (int i = 'A'; i <= 'G'; i++) {
    appendPort(output_ports_, QString(static_cast<char>(i)),
               ports::PortType::Output);
  }
  appendPort(output_ports_, "DP", ports::PortType::Output);
}

COMPONENT_CLASS_DEFINITION(SegmentDisplay, 2, 3)
COMPONENT_CLASS_DEFINITION(FourDigitSegmentDisplay, 6, 3)

void SegmentDisplayComponent::onSettingsBtnClicked() {
  auto dialog = new SegmentDisplaySettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}

void FourDigitSegmentDisplayComponent::onSettingsBtnClicked() {
  auto dialog = new SegmentDisplaySettingsDialog(this, this);
  dialog->exec();
  delete dialog;
}

SegmentDisplaySettingsDialog::SegmentDisplaySettingsDialog(
    AbstractComponent *component, QWidget *parent)
    : ComponentSettingsDialog(component, parent),
      ActiveModeSettingsDialog(component, parent),
      VisionPersistenceSettingsDialog(component, parent) {}

SegmentDisplaySettingsDialog::~SegmentDisplaySettingsDialog() {}

void SegmentDisplaySettingsDialog::acceptDerivedClassSettings() {
  ActiveModeSettingsDialog::acceptDerivedClassSettings();
  VisionPersistenceSettingsDialog::acceptDerivedClassSettings();
}
