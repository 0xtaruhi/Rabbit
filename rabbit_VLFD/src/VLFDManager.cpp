#include "VLFDManager.h"

#include <iostream>

using namespace rabbit_vlfd;

VLFDManager::VLFDManager()
    : handler_(std::make_unique<VLFDHandler>()), freq_(1) {}

VLFDManager::~VLFDManager() {}

void VLFDManager::initDriver() { handler_->initIO(4, 4); }

void VLFDManager::start() {
  runing_thread_ =
      std::make_unique<std::thread>(&VLFDManager::runingThreadFunc, this);
  is_running_ = true;
}

void VLFDManager::stop() {
  is_running_ = false;
  if (runing_thread_->joinable()) {
    runing_thread_->join();
  }
}

void VLFDManager::program(const char *bit_file_path) {
  if (is_running_) {
    throw std::runtime_error("Cannot program while driver is running.");
  }
  handler_->program(bit_file_path);
}

void VLFDManager::setFrequency(uint16_t freq) {
  if (freq > kMaxFrequency) {
    auto err_msg = "Frequency is too high. Max frequency is " +
                   std::to_string(kMaxFrequency) + ".";
    throw std::runtime_error(err_msg);
  } else if (freq == 0) {
    throw std::runtime_error("Frequency cannot be zero.");
  }
  freq_ = freq;
}

void VLFDManager::runingThreadFunc() {
  while (is_running_) {
    // handler_->writeReadData();
    std::cout << "Hello";
    std::flush(std::cout);
    std::this_thread::sleep_for(std::chrono::microseconds(1000000 / freq_));
  }
}
