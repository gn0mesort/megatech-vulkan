/// @cond INTERNAL
/**
 * @file device_impl.hpp
 * @brief Device Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_DEVICE_IMPL_HPP

#include <memory>
#include <unordered_set>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../device.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  class physical_device_description_impl;

  /**
   * @brief The implementation of a megatech::vulkan::device.
   */
  class device_impl final {
  public:
    /**
     * @brief The type of Vulkan handle owned by a device_impl.
     */
    using handle_type = VkDevice;

    /**
     * @brief The parent object type required to construct a device_impl.
     */
    using parent_type = physical_device_description_impl;
  private:
    std::unique_ptr<dispatch::device::table> m_ddt{ };
    std::shared_ptr<const parent_type> m_parent{ };
    VkQueue m_primary_queue{ };
    VkQueue m_async_compute_queue{ };
    VkQueue m_async_transfer_queue{ };
  public:
    /// @cond
    device_impl() = delete;
    /// @endcond

    /**
     * @brief Construct a device_impl.
     * @param parent A shared_ptr to a read-only physical_device_description_impl. This must not be null.
     */
    device_impl(const std::shared_ptr<const parent_type>& parent);

    /// @cond
    device_impl(const device_impl& other) = delete;
    device_impl(device_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a device_impl.
     */
    ~device_impl() noexcept;

    /// @cond
    device_impl& operator=(const device_impl& rhs) = delete;
    device_impl& operator=(device_impl&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the device dispatch table.
     * @return A read-only reference to a dispatch::device::table.
     */
    const dispatch::device::table& dispatch_table() const;

    /**
     * @brief Retrieve the device_impl's underlying Vulkan handle.
     * @return A valid VkDevice.
     */
    handle_type handle() const;

    /**
     * @brief Retrieve the device_impl's parent object.
     * @return A read-only reference to a physical_device_description_impl.
     */
    const parent_type& parent() const;

    /**
     * @brief Retrieve the device_impl's set of enabled extensions.
     * @return A read-only reference to a set of extensions.
     */
    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  static_assert(megatech::vulkan::concepts::readonly_child_object<device_impl>);
  static_assert(megatech::vulkan::concepts::handle_owner<device_impl>);

}

#endif
/// @endcond
