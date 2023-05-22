/*
 * File: VLFDHandler.h
 * Author: 0xtaruhi (zhang_zhengyi@outlook.com)
 * File Created: Friday, 19th May 2023 12:04:54 pm
 * Last Modified: Friday, 19th May 2023 12:05:16 pm
 * Copyright: 2023 - 2023 Fudan University
 */

#ifndef VLFD_HANDLER_H
#define VLFD_HANDLER_H

#include "ProgramVLFD.h"
#include <memory>
#include <vector>

namespace rabbit_vlfd {

/// @brief This class is a wrapper of the VLFD driver. **Never use it directly**.
class VLFDHandler {
public:
  VLFDHandler();
  virtual ~VLFDHandler();

  /// @brief Program VLFD. When the programming is failed, the function will
  /// throw an exception.
  /// @param bit_file_path The path of the bit file.
  void program(const char *bit_file_path);

  /// @brief Check whether the VLFD is programmed.
  /// @return True if the VLFD is programmed, false otherwise.
  bool isProgrammed() const noexcept { return is_programmed_; }

  /// @brief Initialize the IO buffer.
  /// @param write_count write buffer size
  /// @param read_count read buffer size
  /// @note The buffer is implemented as a vertor of uint16_t.
  void initIO(int write_count, int read_count) noexcept;

  /// @brief Open the IO of the VLFD boad;
  void openIO();

  /// @brief Close the IO of the VLFD board.
  void closeIO();

  void writeReadData();

private:
  bool is_programmed_;

  std::vector<uint16_t> write_buf_;
  std::vector<uint16_t> read_buf_;
};
} // namespace rabbit_vlfd

#endif // VLFD_HANDLER_H
