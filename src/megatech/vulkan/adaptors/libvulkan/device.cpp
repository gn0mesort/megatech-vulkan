#include "megatech/vulkan/adaptors/libvulkan/device.hpp"

#include "megatech/vulkan/adaptors/libvulkan/internal/base/device_impl.hpp"

namespace megatech::vulkan::adaptors::libvulkan {

  device::device(const physical_device_description& description) :
  megatech::vulkan::device{ new internal::base::device_impl{ description } } { }

}
