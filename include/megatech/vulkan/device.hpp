#ifndef MEGATECH_VULKAN_DEVICE_HPP
#define MEGATECH_VULKAN_DEVICE_HPP

#include <memory>

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class device_impl;

}

namespace megatech::vulkan {

  class physical_device_description;

  /**
   * @brief A Vulkan device.
   * @details Devices are fully functional Vulkan-capable hardware that are ready to accept commands. It is possible
   *          for an application to instantiate many devices, but a single device is generally servicable.
   */
  class device {
  public:
    using implementation_type = internal::base::device_impl;
  protected:
    std::shared_ptr<implementation_type> m_impl{ };
  public:
    /// @cond
    device() = delete;
    /// @endcond

    /**
     * @brief Construct a device.
     * @param parent A physical_device_description describing the device to construct.
     */
    explicit device(const physical_device_description& parent);

    /// @cond
    device(const device& other) = delete;
    device(device&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a device.
     */
    virtual ~device() noexcept = default;

    /// @cond
    device& operator=(const device& other) = delete;
    device& operator=(device&& other) = delete;
    /// @endcond

    /**
     * @brief Retrieve an opaque reference to the underlying implementation.
     * @return A reference to the underlying implementation.
     */
    const implementation_type& implementation() const;

    /**
     * @brief Retrieve an opaque reference to the underlying implementation.
     * @return A reference to the underlying implementation.
     */
    implementation_type& implementation();

    /**
     * @brief Retrieve a sharable reference to the underlying implementation.
     * @details share_implementation() can be used to extend the life of the underlying implementation object. This
     *          is used, primarily, to ensure valid ownership in dependent objects.
     * @return A shareable reference to the underlying implmentation.
     */
    std::shared_ptr<const implementation_type> share_implementation() const;
  };

  static_assert(concepts::opaque_object<device>);
  static_assert(concepts::readonly_sharable_opaque_object<device>);

}

#endif
