#include "megatech/vulkan/internal/base/device_impl.hpp"

#include <vector>

#include "megatech/vulkan/error.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define DECLARE_DEVICE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN(dt, cmd)
#define DECLARE_DEVICE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  device_description::device_description(const std::unordered_set<std::string>& requested_extensions,
                                         const std::unordered_set<std::string>& required_extensions) :
  m_requested_extensions{ requested_extensions },
  m_required_extensions{ required_extensions } { }

  const std::unordered_set<std::string>& device_description::requested_extensions() const {
    return m_requested_extensions;
  }

  const std::unordered_set<std::string>& device_description::required_extensions() const {
    return m_required_extensions;
  }

  device_impl::device_impl(const std::shared_ptr<const parent_type>& parent, const device_description& description) :
  m_parent{ parent } {
    auto device_info = VkDeviceCreateInfo{ };
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    m_enabled_extensions.insert("VK_KHR_dynamic_rendering_local_read");
    for (const auto& extension : description.required_extensions())
    {
      m_enabled_extensions.insert(extension);
    }
    for (const auto& extension : description.requested_extensions())
    {
      if (m_parent->available_extensions().contains(extension))
      {
        m_enabled_extensions.insert(extension);
      }
    }
    auto enabled_extensions = std::vector<const char*>{ };
    for (const auto& extension : m_enabled_extensions)
    {
      enabled_extensions.emplace_back(extension.data());
    }
    device_info.enabledExtensionCount = enabled_extensions.size();
    device_info.ppEnabledExtensionNames = enabled_extensions.data();
    auto features = VkPhysicalDeviceFeatures2{ };
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    auto features_1_3 = VkPhysicalDeviceVulkan13Features{ };
    features_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    features_1_3.dynamicRendering = VK_TRUE;
    auto features_dynamic_rendering_local_read = VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR{ };
    features_dynamic_rendering_local_read.sType =
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
    features.pNext = &features_1_3;
    features_dynamic_rendering_local_read.dynamicRenderingLocalRead = VK_TRUE;
    features_1_3.pNext = &features_dynamic_rendering_local_read;
    device_info.pNext = &features;
    const auto priority = 1.0f;
    auto queue_infos = std::vector<VkDeviceQueueCreateInfo>(3);
    for (auto& queue_info : queue_infos)
    {
      queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_info.pQueuePriorities = &priority;
      queue_info.queueCount = 1;
    }
    queue_infos[0].queueFamilyIndex = m_parent->primary_queue_family_index();
    queue_infos[1].queueFamilyIndex = m_parent->async_compute_queue_family_index();
    queue_infos[2].queueFamilyIndex = m_parent->async_transfer_queue_family_index();
    device_info.queueCreateInfoCount = 1 + (m_parent->async_compute_queue_family_index() != -1) +
                                       (m_parent->async_transfer_queue_family_index() != -1);
    device_info.pQueueCreateInfos = queue_infos.data();
    DECLARE_INSTANCE_PFN(m_parent->parent().dispatch_table(), vkCreateDevice);
    VK_CHECK(vkCreateDevice(m_parent->handle(), &device_info, nullptr,&m_device));
    m_ddt.reset(new dispatch::device::table{ m_parent->parent().parent().dispatch_table(),
                                             m_parent->parent().dispatch_table(), m_device });
    DECLARE_DEVICE_PFN(*m_ddt, vkGetDeviceQueue);
    vkGetDeviceQueue(m_device, m_parent->primary_queue_family_index(), 0, &m_primary_queue);
    if (m_parent->async_compute_queue_family_index() != -1)
    {
      vkGetDeviceQueue(m_device, m_parent->async_compute_queue_family_index(), 0, &m_async_compute_queue);
    }
    if (m_parent->async_transfer_queue_family_index() != -1)
    {
      vkGetDeviceQueue(m_device, m_parent->async_transfer_queue_family_index(), 0, &m_async_transfer_queue);
    }
  }

  device_impl::~device_impl() noexcept {
    DECLARE_DEVICE_PFN_NO_THROW(*m_ddt, vkDeviceWaitIdle);
    vkDeviceWaitIdle(m_device);
    DECLARE_DEVICE_PFN_NO_THROW(*m_ddt, vkDestroyDevice);
    vkDestroyDevice(m_device, nullptr);
  }

  const dispatch::device::table& device_impl::dispatch_table() const {
    return *m_ddt;
  }

  device_impl::handle_type device_impl::handle() const {
    return m_device;
  }

  const device_impl::parent_type& device_impl::parent() const {
    return *m_parent;
  }

  const std::unordered_set<std::string>& device_impl::enabled_extensions() const {
    return m_enabled_extensions;
  }

}
