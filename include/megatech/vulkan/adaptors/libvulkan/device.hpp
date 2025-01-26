#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_DEVICE_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_DEVICE_HPP

#include "../../device.hpp"

namespace megatech::vulkan {

  class physical_device_description;

}

namespace megatech::vulkan::adaptors::libvulkan {

  /**
   * @brief A Vulkan device.
   */
  class device final : public megatech::vulkan::device {
  public:
    /**
     * @brief Construct a device.
     * @param description The description of the physical device to initialize.
     */
    explicit device(const physical_device_description& description);

    /// @cond
    device(const device& other) = delete;
    device(device&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a device.
     */
    ~device() noexcept = default;

    /// @cond
    device& operator=(const device& rhs) = delete;
    device& operator=(device&& rhs) = delete;
    /// @endcond
  };

}

#endif
