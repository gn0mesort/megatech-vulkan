#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/error.hpp"

namespace megatech::vulkan {

  error::error(const std::string& message, const std::source_location& location) :
  error{ message, VK_ERROR_UNKNOWN, location } { }

  error::error(const std::string& message, const VkResult vk_result, const std::source_location& location) :
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

  VkResult error::vk_result() const noexcept {
    return static_cast<VkResult>(m_vk_result);
  }

}
