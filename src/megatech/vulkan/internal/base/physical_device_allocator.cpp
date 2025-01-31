#include "megatech/vulkan/internal/base/physical_device_allocator.hpp"

#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

namespace megatech::vulkan::internal::base {

  physical_device_allocator::result_type*
  physical_device_allocator::allocate(const std::shared_ptr<const parent_type>& parent, handle_type handle) const {
    return new result_type{ parent, handle };
  }

  physical_device_allocator::result_type*
  physical_device_allocator::operator()(const std::shared_ptr<const parent_type>& parent, handle_type handle) const {
    return allocate(parent, handle);
  }


}
