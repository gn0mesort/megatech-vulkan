#include "megatech/vulkan/internal/base/device_impl.hpp"

#include <vector>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/error.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define DECLARE_DEVICE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN(dt, cmd)
#define DECLARE_DEVICE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  device_impl::device_impl(const std::shared_ptr<const parent_type>& parent) :
  m_parent{ parent } {
    if (!parent)
    {
      throw error{ "The parent physical_device_description cannot be null." };
    }
    auto device_info = VkDeviceCreateInfo{ };
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    auto enabled_extensions = std::vector<const char*>{ };
    for (const auto& extension : m_parent->required_extensions())
    {
      enabled_extensions.emplace_back(extension.data());
    }
    device_info.enabledExtensionCount = enabled_extensions.size();
    device_info.ppEnabledExtensionNames = enabled_extensions.data();
    device_info.pNext = &m_parent->required_features();
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
    auto device = VkDevice{ };
    VK_CHECK(vkCreateDevice(m_parent->handle(), &device_info, nullptr,&device));
    m_ddt.reset(new dispatch::device::table{ m_parent->parent().parent().dispatch_table(),
                                             m_parent->parent().dispatch_table(), device });
    DECLARE_DEVICE_PFN(*m_ddt, vkGetDeviceQueue);
    vkGetDeviceQueue(m_ddt->device(), m_parent->primary_queue_family_index(), 0, &m_primary_queue);
    if (m_parent->async_compute_queue_family_index() != -1)
    {
      vkGetDeviceQueue(m_ddt->device(), m_parent->async_compute_queue_family_index(), 0, &m_async_compute_queue);
    }
    if (m_parent->async_transfer_queue_family_index() != -1)
    {
      vkGetDeviceQueue(m_ddt->device(), m_parent->async_transfer_queue_family_index(), 0, &m_async_transfer_queue);
    }
    MEGATECH_POSTCONDITION(m_parent != nullptr);
    MEGATECH_POSTCONDITION(m_parent == parent);
    MEGATECH_POSTCONDITION(m_ddt != nullptr);
    MEGATECH_POSTCONDITION(m_ddt->device() == device);
    MEGATECH_POSTCONDITION(m_primary_queue != VK_NULL_HANDLE);
  }

  device_impl::~device_impl() noexcept {
    DECLARE_DEVICE_PFN_NO_THROW(*m_ddt, vkDeviceWaitIdle);
    vkDeviceWaitIdle(m_ddt->device());
    DECLARE_DEVICE_PFN_NO_THROW(*m_ddt, vkDestroyDevice);
    vkDestroyDevice(m_ddt->device(), nullptr);
  }

  const dispatch::device::table& device_impl::dispatch_table() const {
    MEGATECH_PRECONDITION(m_ddt != nullptr);
    return *m_ddt;
  }

  device_impl::handle_type device_impl::handle() const {
    MEGATECH_PRECONDITION(m_ddt != nullptr);
    return m_ddt->device();
  }

  const device_impl::parent_type& device_impl::parent() const {
    MEGATECH_PRECONDITION(m_parent != nullptr);
    return *m_parent;
  }

  const std::unordered_set<std::string>& device_impl::enabled_extensions() const {
    MEGATECH_PRECONDITION(m_parent != nullptr);
    return m_parent->required_extensions();
  }

}
