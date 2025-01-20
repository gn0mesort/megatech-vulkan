#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP

#include <unordered_set>

#include "../../physical_devices.hpp"

#include "../../defs.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;

  class physical_device_description_impl final {
  private:
    std::shared_ptr<const instance_impl> m_parent{ };
    VkPhysicalDevice m_handle{ };
    VkPhysicalDeviceProperties m_properties_1_0{ };
    VkPhysicalDeviceVulkan11Properties m_properties_1_1{ };
    VkPhysicalDeviceVulkan12Properties m_properties_1_2{ };
    VkPhysicalDeviceVulkan13Properties m_properties_1_3{ };
    VkPhysicalDeviceFeatures m_features_1_0{ };
    VkPhysicalDeviceVulkan11Features m_features_1_1{ };
    VkPhysicalDeviceVulkan12Features m_features_1_2{ };
    VkPhysicalDeviceVulkan13Features m_features_1_3{ };
    std::vector<VkQueueFamilyProperties> m_queue_family_properties{ };
    std::unordered_set<std::string> m_available_extensions{ };

    int64_t m_primary_queue_family{ -1 };
    int64_t m_async_compute_queue_family{ -1 };
    int64_t m_async_transfer_queue_family{ -1 };
  public:
    using handle_type = VkPhysicalDevice;
    using parent_type = instance_impl;

    physical_device_description_impl() = delete;
    physical_device_description_impl(std::shared_ptr<const instance_impl> parent, VkPhysicalDevice handle);
    physical_device_description_impl(const physical_device_description_impl& other) = delete;
    physical_device_description_impl(physical_device_description_impl&& other) = delete;

    ~physical_device_description_impl() noexcept = default;

    physical_device_description_impl& operator=(const physical_device_description_impl& rhs) = delete;
    physical_device_description_impl& operator=(physical_device_description_impl&& rhs) = delete;

    handle_type handle() const;
    const parent_type& parent() const;
    const VkPhysicalDeviceProperties& properties_1_0() const;
    const VkPhysicalDeviceVulkan11Properties& properties_1_1() const;
    const VkPhysicalDeviceVulkan12Properties& properties_1_2() const;
    const VkPhysicalDeviceVulkan13Properties& properties_1_3() const;
    const VkPhysicalDeviceFeatures& features_1_0() const;
    const VkPhysicalDeviceVulkan11Features& features_1_1() const;
    const VkPhysicalDeviceVulkan12Features& features_1_2() const;
    const VkPhysicalDeviceVulkan13Features& features_1_3() const;
    const std::unordered_set<std::string>& available_extensions() const;
    const std::vector<VkQueueFamilyProperties>& queue_family_properties() const;
    int64_t primary_queue_family_index() const;
    int64_t async_compute_queue_family_index() const;
    int64_t async_transfer_queue_family_index() const;
    const VkQueueFamilyProperties& primary_queue_family_properties() const;
    const VkQueueFamilyProperties& async_compute_queue_family_properties() const;
    const VkQueueFamilyProperties& async_transfer_queue_family_properties() const;
  };

  static_assert(concepts::readonly_child_object<physical_device_description_impl>);
  static_assert(concepts::handle_owner<physical_device_description_impl>);

}

#endif
