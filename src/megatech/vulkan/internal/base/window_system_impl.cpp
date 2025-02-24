/**
 * @file window_system_impl.cpp
 * @brief Window System Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/internal/base/window_system_impl.hpp"

#include "megatech/vulkan/error.hpp"

namespace megatech::vulkan::internal::base {

  const std::unordered_set<std::string>& window_system_impl::required_extensions() const {
    return m_required_extensions;
  }

  bool window_system_impl::can_check_presentation_support() const {
    return false;
  }

  bool window_system_impl::is_presentation_supported(const VkPhysicalDevice, const std::uint32_t) const {
    throw error{ "Checking for presentation support without a surface is not supported by default." };
  }

}
