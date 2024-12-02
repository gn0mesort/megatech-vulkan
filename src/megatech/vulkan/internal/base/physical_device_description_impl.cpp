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
    VK_DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkGetPhysicalDeviceQueueFamilyProperties);
    {
      auto sz = std::uint32_t{ 0 };
      vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &sz, nullptr);
      m_queue_family_properties.resize(sz);
      vkGetPhysicalDeviceQueueFamilyProperties(m_handle, &sz, m_queue_family_properties.data());
    }
    {
      switch (m_properties_1_0.vendorID)
      {
      // Nvidia Devices
      case 0x10de:
      // AMD Devices
      case 0x1002:
      // Intel Devices
      case 0x8086:
      // llvmpipe and Other Mesa Devices
      case VK_VENDOR_ID_MESA:
        m_primary_queue_family = !((m_queue_family_properties[0].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0) * -1;
        for (auto i = std::uint32_t{ 1 }; i < m_queue_family_properties.size(); ++i)
        {
          const auto has_compute = m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT;
          const auto has_transfer_only = (!(m_queue_family_properties[i].queueFlags &
                                            (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) &&
                                         (m_queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT));
          m_async_compute_queue_family = (m_async_compute_queue_family != -1) *
                                         (has_compute * i) + (!has_compute * -1);
          m_async_transfer_queue_family = (m_async_transfer_queue_family != -1) *
                                          (has_transfer_only * i) + (!has_transfer_only * -1);
        }
        break;
      default:
        for (auto i = std::uint32_t{ 0 }; i < m_queue_family_properties.size(); ++i)
        {
          constexpr auto compute_graphics = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
          if ((m_queue_family_properties[i].queueFlags & compute_graphics) == compute_graphics)
          {
            if (m_primary_queue_family != -1 &&
                !(m_queue_family_properties[m_primary_queue_family].queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
              m_primary_queue_family = i;
            }
          }
          else if (m_primary_queue_family == -1 && (m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
          {
            m_primary_queue_family = i;
          }
        }
        for (auto i = std::uint32_t{ 0 }; i < m_queue_family_properties.size(); ++i)
        {
          if (m_primary_queue_family == i)
          {
            continue;
          }
          const auto has_compute = m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT;
          const auto has_transfer_only = (!(m_queue_family_properties[i].queueFlags &
                                            (VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT)) &&
                                         (m_queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT));
          m_async_compute_queue_family = (m_async_compute_queue_family != -1) *
                                         (has_compute * i) + (!has_compute * -1);
          m_async_transfer_queue_family = (m_async_transfer_queue_family != -1) *
                                          (has_transfer_only * i) + (!has_transfer_only * -1);
        }
        break;
      }
    }
  }

  physical_device_description_impl::handle_type physical_device_description_impl::handle() const {
    return m_handle;
  }

  const physical_device_description_impl::parent_type& physical_device_description_impl::parent() const {
    return *m_parent;
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

  const std::vector<VkQueueFamilyProperties>& physical_device_description_impl::queue_family_properties() const {
    return m_queue_family_properties;
  }

  int64_t physical_device_description_impl::primary_queue_family_index() const {
    return m_primary_queue_family;
  }

  int64_t physical_device_description_impl::async_compute_queue_family_index() const {
    return m_async_compute_queue_family;
  }

  int64_t physical_device_description_impl::async_transfer_queue_family_index() const {
    return m_async_transfer_queue_family;
  }

  const VkQueueFamilyProperties& physical_device_description_impl::primary_queue_family_properties() const {
    if (m_primary_queue_family == -1)
    {
      // TODO: throw
    }
    return m_queue_family_properties[m_primary_queue_family];
  }

  const VkQueueFamilyProperties& physical_device_description_impl::async_compute_queue_family_properties() const {
    if (m_async_compute_queue_family == -1)
    {
      // TODO: throw
    }
    return m_queue_family_properties[m_async_compute_queue_family];
  }

  const VkQueueFamilyProperties& physical_device_description_impl::async_transfer_queue_family_properties() const {
    if (m_async_transfer_queue_family == -1)
    {
      // TODO: throw
    }
    return m_queue_family_properties[m_async_transfer_queue_family];
  }

}
