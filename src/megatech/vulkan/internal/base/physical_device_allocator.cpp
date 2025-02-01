/// @cond INTERNAL
/**
 * @file physical_device_allocator.cpp
 * @brief Indirect Physical Device Object Allocator
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/internal/base/physical_device_allocator.hpp"

#include <megatech/assertions.hpp>

#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

namespace megatech::vulkan::internal::base {

  physical_device_allocator::result_type*
  physical_device_allocator::allocate(const std::shared_ptr<const parent_type>& parent, handle_type handle) const {
    MEGATECH_PRECONDITION(parent != nullptr);
    MEGATECH_PRECONDITION(handle != VK_NULL_HANDLE);
    return new result_type{ parent, handle };
  }

  physical_device_allocator::result_type*
  physical_device_allocator::operator()(const std::shared_ptr<const parent_type>& parent, handle_type handle) const {
    MEGATECH_PRECONDITION(parent != nullptr);
    MEGATECH_PRECONDITION(handle != VK_NULL_HANDLE);
    return allocate(parent, handle);
  }

}
/// @endcond
