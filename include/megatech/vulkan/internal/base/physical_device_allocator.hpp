#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_ALLOCATOR_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_ALLOCATOR_HPP

#include <memory>

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class physical_device_description_impl;

  class physical_device_allocator {
  public:
    using handle_type = VkPhysicalDevice;
    using parent_type = instance_impl;
    using result_type = physical_device_description_impl;

    physical_device_allocator() = default;
    physical_device_allocator(const physical_device_allocator& other) = default;
    physical_device_allocator(physical_device_allocator&& other) = default;

    virtual ~physical_device_allocator() noexcept = default;

    physical_device_allocator& operator=(const physical_device_allocator& rhs) = default;
    physical_device_allocator& operator=(physical_device_allocator&& rhs) = default;

    virtual result_type* allocate(const std::shared_ptr<const parent_type>& parent, handle_type handle) const;
    result_type* operator()(const std::shared_ptr<const parent_type>& parent, handle_type handle) const;
  };

}

#endif
