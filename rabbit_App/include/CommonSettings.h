#pragma once
#ifndef COMMON_SETTINGS_H

#include <QSettings>

namespace rabbit_App {

class CommonSettings {
public:
  CommonSettings() : settings_("rabbit_App", "rabbit_App") {}

  static auto getInstance() -> CommonSettings & {
    static CommonSettings instance;
    return instance;
  }

  auto getSettingsValue(const QString &key) const noexcept {
    return settings_.value(key);
  }

  auto updateSettingsValue(const QString &key, const QVariant &value) noexcept {
    settings_.setValue(key, value);
  }

private:
  QSettings settings_;
};
} // namespace rabbit_App

#endif // COMMON_SETTINGS_H
