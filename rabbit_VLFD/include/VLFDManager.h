#ifndef VLFD_MANAGER_H
#define VLFD_MANAGER_H

#include "VLFDHandler.h"
#include <thread>

namespace rabbit_vlfd {
class VLFDManager {
public:
  VLFDManager();

  ~VLFDManager();

  void initDriver();
  void start();
  void stop();
  void program(const char *bit_file_path);

  static const uint16_t kMaxFrequency = 10000;
  void setFrequency(uint16_t freq);

  auto &getDriverHandler() noexcept { return handler_; }
  auto getFrequency() const noexcept { return freq_; }
  auto running() const noexcept { return is_running_; }

private:
  std::unique_ptr<VLFDHandler> handler_;
  bool is_running_;
  uint16_t freq_;
  std::unique_ptr<std::thread> runing_thread_;

  void runingThreadFunc();
};
} // namespace rabbit_vlfd

#endif // VLFD_MANAGER_H
