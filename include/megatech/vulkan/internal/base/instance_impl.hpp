#ifndef MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP

#include <memory>
#include <vector>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../instance.hpp"

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::internal::base {

  class loader_impl;

  class instance_impl : public std::enable_shared_from_this<instance_impl> {
  protected:
    instance_impl() = default;

    std::unique_ptr<dispatch::instance::table> m_idt{ };
    std::shared_ptr<const loader_impl> m_parent{ };
    VkInstance m_instance{ };
  public:
    instance_impl(const instance_impl& other) = delete;
    instance_impl(instance_impl&& other) = delete;

    virtual ~instance_impl() noexcept = default;

    instance_impl& operator=(const instance_impl& rhs) = delete;
    instance_impl& operator=(instance_impl&& rhs) = delete;

    virtual bool is_valid(const physical_device_description& physical_device) const;

    const dispatch::instance::table& dispatch_table() const;
    VkInstance handle() const;
  };

}

#endif
