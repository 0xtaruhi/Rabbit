#pragma once
#ifndef COMPONENT_MACRO_H
#define COMPONENT_MACRO_h

#define COMPONENT_CLASS_DECLARATION(type, component_name)                      \
  class component_name##RawComponent : public QWidget {                        \
    Q_OBJECT                                                                   \
  public:                                                                      \
    component_name##RawComponent(QWidget *parent = nullptr);                   \
    ~component_name##RawComponent();                                           \
                                                                               \
  protected:                                                                   \
    void paintEvent(QPaintEvent *event) override;                              \
                                                                               \
  private:                                                                     \
    component_name##_RAW_EXTRA_MEMBERS                                         \
  };                                                                           \
                                                                               \
  class component_name##Component : public Abstract##type##Component {         \
    Q_OBJECT                                                                   \
  public:                                                                      \
    component_name##Component(QWidget *parent = nullptr);                      \
    ~component_name##Component();                                              \
    virtual QSize gridOccupation() const noexcept override;                    \
  };

#endif // COMPONENT_MACRO_H
