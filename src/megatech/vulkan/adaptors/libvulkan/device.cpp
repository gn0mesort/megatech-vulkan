#include "megatech/vulkan/adaptors/libvulkan/device.hpp"

#include "megatech/vulkan/physical_devices.hpp"

#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"
#include "megatech/vulkan/internal/base/device_impl.hpp"

namespace mv = megatech::vulkan;
namespace mvib = megatech::vulkan::internal::base;

namespace megatech::vulkan::adaptors::libvulkan {

  device::device(const physical_device_description& description) :
  mv::device{ [](const physical_device_description& description) {
    auto ptr = new implementation_type{ description.share_implementation(), mvib::device_description{ } };
    return std::shared_ptr<implementation_type>{ ptr };
  }(description) } { }

}
