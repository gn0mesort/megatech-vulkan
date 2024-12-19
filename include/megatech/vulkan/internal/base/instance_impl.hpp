#ifndef MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP

#include <memory>
#include <unordered_set>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../defs.hpp"
#include "../../instance.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::internal::base {

  class loader_impl;

  class instance_impl {
  protected:
    instance_impl() = default;

    std::unique_ptr<dispatch::instance::table> m_idt{ };
    std::shared_ptr<const loader_impl> m_parent{ };
    VkInstance m_instance{ };
    std::unordered_set<std::string> m_enabled_layers{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  public:
    using handle_type = VkInstance;
    using parent_type = loader_impl;

    instance_impl(const instance_impl& other) = delete;
    instance_impl(instance_impl&& other) = delete;

    virtual ~instance_impl() noexcept = default;

    instance_impl& operator=(const instance_impl& rhs) = delete;
    instance_impl& operator=(instance_impl&& rhs) = delete;

    virtual bool is_valid(const physical_device_description& physical_device) const;

    const dispatch::instance::table& dispatch_table() const;
    handle_type handle() const;

    const parent_type& parent() const;

    const std::unordered_set<std::string>& enabled_layers() const;
    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  MEGATECH_VULKAN_ENFORCE_CONCEPT(megatech::vulkan::concepts::readonly_child_object<instance_impl>);
  MEGATECH_VULKAN_ENFORCE_CONCEPT(megatech::vulkan::concepts::handle_owner<instance_impl>);

}

#endif
