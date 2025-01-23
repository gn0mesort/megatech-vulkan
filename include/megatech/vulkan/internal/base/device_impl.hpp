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

  class device_description final {
  private:
    std::unordered_set<std::string> m_requested_extensions{ };
    std::unordered_set<std::string> m_required_extensions{ };
  public:
    device_description() = default;
    device_description(const std::unordered_set<std::string>& requested_extensions,
                       const std::unordered_set<std::string>& required_extensions);
    device_description(const device_description& other) = default;
    device_description(device_description&& other) = default;

    ~device_description() noexcept = default;

    device_description& operator=(const device_description& other) = default;
    device_description& operator=(device_description&& other) = default;

    const std::unordered_set<std::string>& requested_extensions() const;
    const std::unordered_set<std::string>& required_extensions() const;
  };

  class device_impl final {
  public:
    using handle_type = VkDevice;
    using parent_type = physical_device_description_impl;
  protected:
    std::unique_ptr<dispatch::device::table> m_ddt{ };
    std::shared_ptr<const parent_type> m_parent{ };
    VkDevice m_device{ };
    VkQueue m_primary_queue{ };
    VkQueue m_async_compute_queue{ };
    VkQueue m_async_transfer_queue{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  public:
    device_impl() = delete;
    device_impl(const std::shared_ptr<const parent_type>& parent, const device_description& description);
    device_impl(const device_impl& other) = delete;
    device_impl(device_impl&& other) = delete;

    ~device_impl() noexcept;

    device_impl& operator=(const device_impl& rhs) = delete;
    device_impl& operator=(device_impl&& rhs) = delete;

    const dispatch::device::table& dispatch_table() const;
    handle_type handle() const;
    const parent_type& parent() const;

    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  static_assert(megatech::vulkan::concepts::readonly_child_object<device_impl>);
  static_assert(megatech::vulkan::concepts::handle_owner<device_impl>);

}

#endif
