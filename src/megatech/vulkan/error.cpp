/**
 * @file error.cpp
 * @brief Vulkan Errors
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/error.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"

namespace megatech::vulkan {

  error::error(const std::string& message, const std::source_location& location) :
  error{ message, VK_ERROR_UNKNOWN, location } { }

  error::error(const std::string& message, const std::int_least64_t vk_result, const std::source_location& location) :
  m_vk_result{ vk_result }, m_message{ message }, m_location{ location } { }

  const std::string& error::message() const noexcept {
    return m_message;
  }

  const char* error::what() const noexcept {
    return m_message.data();
  }

  const std::source_location& error::location() const noexcept {
    return m_location;
  }

  std::int_least64_t error::vk_result() const noexcept {
    return m_vk_result;
  }

}
