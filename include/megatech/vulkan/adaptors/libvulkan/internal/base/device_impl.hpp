#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP

#include "../../../../internal/base/device_impl.hpp"

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  class device_impl final : public megatech::vulkan::internal::base::device_impl {
  public:
    explicit device_impl(const physical_device_description& description);
    device_impl(const device_impl& other) = delete;
    device_impl(device_impl&& other) = delete;

    ~device_impl() noexcept;

    device_impl& operator=(const device_impl& rhs) = delete;
    device_impl& operator=(device_impl&& rhs) = delete;
  };

}

#endif
