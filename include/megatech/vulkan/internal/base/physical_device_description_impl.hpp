#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP

#include "../../physical_devices.hpp"

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
  public:
    physical_device_description_impl() = delete;
    physical_device_description_impl(std::shared_ptr<const instance_impl> parent, VkPhysicalDevice handle);
    physical_device_description_impl(const physical_device_description_impl& other) = delete;
    physical_device_description_impl(physical_device_description_impl&& other) = delete;

    ~physical_device_description_impl() noexcept = default;

    physical_device_description_impl& operator=(const physical_device_description_impl& rhs) = delete;
    physical_device_description_impl& operator=(physical_device_description_impl&& rhs) = delete;

    VkPhysicalDevice handle() const;
    const VkPhysicalDeviceProperties& properties_1_0() const;
    const VkPhysicalDeviceVulkan11Properties& properties_1_1() const;
    const VkPhysicalDeviceVulkan12Properties& properties_1_2() const;
    const VkPhysicalDeviceVulkan13Properties& properties_1_3() const;
  };

}

#endif
