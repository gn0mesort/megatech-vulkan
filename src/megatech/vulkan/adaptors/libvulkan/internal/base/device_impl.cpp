#include "megatech/vulkan/adaptors/libvulkan/internal/base/device_impl.hpp"

#include <array>

#include "megatech/vulkan/physical_devices.hpp"

#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  device_impl::device_impl(const physical_device_description& description) {
    m_parent = description.share_implementation();
    auto device_info = VkDeviceCreateInfo{ };
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    // This is safe because it's validated earlier.
    auto selected_extensions = std::array<const char*, 1>{ "VK_KHR_dynamic_rendering_local_read" };
    device_info.
  }

  device_impl::~device_impl() noexcept {
  }

}
