#pragma once
#ifndef COMPONENT_MACRO_H
#define COMPONENT_MACRO_h

#define COMPONENT_CLASS_DECLARATION(component_name)                            \
  class component_name##Component : public AbstractComponent {                 \
    Q_OBJECT                                                                   \
  public:                                                                      \
    component_name##Component(QWidget *parent = nullptr);                      \
    virtual ~component_name##Component();                                      \
                                                                               \
    QSize gridOccupation() const noexcept override;                            \
    QString componentType() const noexcept override {                          \
      return QString(#component_name);                                         \
    }                                                                          \
    static QIcon componentIcon() noexcept {                                    \
      return QIcon(":/icons/icons/components/" #component_name "-icon.png");   \
    }                                                                          \
                                                                               \
  protected:                                                                   \
    void contextMenuEvent(QContextMenuEvent *event) override;                  \
  protected slots:                                                             \
    void onSettingsBtnClicked() override;                                      \
  };

#define COMPONENT_CLASS_DEFINITION(component_name, x, y)                       \
  component_name##Component::component_name##Component(QWidget *parent)        \
      : AbstractComponent(parent) {                                            \
    raw_component_ = new component_name##RawComponent(this);                   \
    layout()->addWidget(raw_component_);                                       \
  }                                                                            \
                                                                               \
  component_name##Component::~component_name##Component() {}                   \
                                                                               \
  QSize component_name##Component::gridOccupation() const noexcept {           \
    return {x, y};                                                             \
  }                                                                            \
                                                                               \
  void component_name##Component::contextMenuEvent(QContextMenuEvent *event) { \
    AbstractComponent::contextMenuEvent(event);                                \
  }

#endif // COMPONENT_MACRO_H
