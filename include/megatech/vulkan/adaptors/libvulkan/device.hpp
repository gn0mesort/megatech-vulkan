#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_DEVICE_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_DEVICE_HPP

#include "../../device.hpp"

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::adaptors::libvulkan {

  class device final : public megatech::vulkan::device {
  public:
    explicit device(const physical_device_description& description);
    device(const device& other) = delete;
    device(device&& other) = delete;

    ~device() noexcept = default;

    device& operator=(const device& rhs) = delete;
    device& operator=(device&& rhs) = delete;
  };

}

#endif
