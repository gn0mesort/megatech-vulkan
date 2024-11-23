#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"


#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"

#define VK_DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  physical_device_description_impl::physical_device_description_impl(std::shared_ptr<const instance_impl> parent,
                                                                     VkPhysicalDevice handle) :
  m_parent{ parent },
  m_handle{ handle } {
    auto properties2 = VkPhysicalDeviceProperties2{ };
    properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    m_properties_1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
    properties2.pNext = &m_properties_1_1;
    m_properties_1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
    m_properties_1_1.pNext = &m_properties_1_2;
    m_properties_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
    m_properties_1_2.pNext = &m_properties_1_3;
    m_properties_1_3.pNext = nullptr;
    VK_DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkGetPhysicalDeviceProperties2);
    vkGetPhysicalDeviceProperties2(m_handle, &properties2);
    m_properties_1_2.pNext = nullptr;
    m_properties_1_1.pNext = nullptr;
    m_properties_1_0 = properties2.properties;
  }

  VkPhysicalDevice physical_device_description_impl::handle() const {
    return m_handle;
  }

  const VkPhysicalDeviceProperties& physical_device_description_impl::properties_1_0() const {
    return m_properties_1_0;
  }

  const VkPhysicalDeviceVulkan11Properties& physical_device_description_impl::properties_1_1() const {
    return m_properties_1_1;
  }

  const VkPhysicalDeviceVulkan12Properties& physical_device_description_impl::properties_1_2() const {
    return m_properties_1_2;
  }

  const VkPhysicalDeviceVulkan13Properties& physical_device_description_impl::properties_1_3() const {
    return m_properties_1_3;
  }

}
