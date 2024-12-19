#ifndef MEGATECH_VULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP

#include <memory>
#include <unordered_set>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../defs.hpp"
#include "../../device.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

namespace megatech::vulkan::internal::base {

  class physical_device_description_impl;

  class device_impl {
  protected:
    device_impl() = default;

    std::unique_ptr<dispatch::device::table> m_ddt{ };
    std::shared_ptr<const physical_device_description_impl> m_parent{ };
    VkDevice m_device{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  public:
    using handle_type = VkDevice;
    using parent_type = physical_device_description_impl;

    device_impl(const device_impl& other) = delete;
    device_impl(device_impl&& other) = delete;

    virtual ~device_impl() noexcept = default;

    device_impl& operator=(const device_impl& rhs) = delete;
    device_impl& operator=(device_impl&& rhs) = delete;

    const dispatch::device::table& dispatch_table() const;
    handle_type handle() const;
    const parent_type& parent() const;

    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  MEGATECH_VULKAN_ENFORCE_CONCEPT(megatech::vulkan::concepts::readonly_child_object<device_impl>);
  MEGATECH_VULKAN_ENFORCE_CONCEPT(megatech::vulkan::concepts::handle_owner<device_impl>);

}

#endif
