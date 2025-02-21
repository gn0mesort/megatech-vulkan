/**
 * @file physical_devices.cpp
 * @brief Vulkan Physical Devices
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/physical_devices.hpp"

#include <cinttypes>

#include <utility>
#include <algorithm>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/instance.hpp"
#include "megatech/vulkan/version.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan {

  physical_device_description::physical_device_description(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  bool physical_device_description::operator==(const physical_device_description& rhs) const noexcept {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl->handle() == rhs.implementation().handle();
  }

  const physical_device_description::implementation_type& physical_device_description::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  physical_device_description::implementation_type& physical_device_description::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const physical_device_description::implementation_type> physical_device_description::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

  bool physical_device_description::supports_rendering() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl->primary_queue_family_properties().queueFlags & VK_QUEUE_GRAPHICS_BIT;
  }

  bool physical_device_description::supports_presentation(const surface&) const {
    return supports_rendering() /* && TODO: check surface support */;
  }

  bool physical_device_description::supports_async_execution() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl->async_transfer_queue_family_index() != -1;
  }

  int physical_device_description::supports_async_transfer() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    if (m_impl->async_transfer_queue_family_index() != -1)
    {
      return async_transfer_support::dedicated;
    }
    if (m_impl->async_compute_queue_family_index() != -1)
    {
      return async_transfer_support::combined;
    }
    return async_transfer_support::none;
  }

  physical_device_list::physical_device_list(std::vector<physical_device_description>&& filtered_list) :
  m_physical_devices{ std::move(filtered_list) } {
    MEGATECH_POSTCONDITION(static_cast<std::size_t>(std::ranges::count_if(m_physical_devices,
                          [](const auto& p){ return p.implementation().is_valid(); })) == m_physical_devices.size());
  }

  physical_device_list::physical_device_list(const instance& inst) {
    auto parent = inst.share_implementation();
    DECLARE_INSTANCE_PFN(parent->dispatch_table(), vkEnumeratePhysicalDevices);
    auto sz = std::uint32_t{ 0 };
    VK_CHECK(vkEnumeratePhysicalDevices(parent->handle(), &sz, nullptr));
    auto handles = std::vector<VkPhysicalDevice>(sz);
    VK_CHECK(vkEnumeratePhysicalDevices(parent->handle(), &sz, handles.data()));
    m_physical_devices.reserve(sz);
    for (auto&& handle : handles)
    {
      using implementation_type = internal::base::physical_device_description_impl;
      auto tmp = std::shared_ptr<implementation_type>{ parent->resolve_physical_device_description(handle) };
      if (tmp->is_valid())
      {
        m_physical_devices.emplace_back(tmp);
      }
    }
    m_physical_devices.shrink_to_fit();
    MEGATECH_POSTCONDITION(static_cast<std::size_t>(std::ranges::count_if(m_physical_devices,
                          [](const auto& p){ return p.implementation().is_valid(); })) == m_physical_devices.size());
  }

  physical_device_list::const_reference physical_device_list::operator[](const physical_device_list::size_type index) const {
    MEGATECH_PRECONDITION(index < m_physical_devices.size());
    return m_physical_devices[index];
  }

  bool physical_device_list::operator==(const physical_device_list& rhs) const noexcept {
    if (m_physical_devices.size() != rhs.size())
    {
      return false;
    }
    for (auto i = physical_device_list::size_type{ 0 }; i < m_physical_devices.size(); ++i)
    {
      if (m_physical_devices[i] != rhs[i])
      {
          return false;
      }
    }
    return true;
  }

  physical_device_list::const_reference physical_device_list::at(const physical_device_list::size_type index) const {
    MEGATECH_PRECONDITION(index < m_physical_devices.size());
    return m_physical_devices.at(index);
  }

  physical_device_list::const_reference physical_device_list::front() const {
    MEGATECH_PRECONDITION(!m_physical_devices.empty());
    return m_physical_devices.front();
  }

  physical_device_list::const_reference physical_device_list::back() const {
    MEGATECH_PRECONDITION(!m_physical_devices.empty());
    return m_physical_devices.back();
  }

  physical_device_list::const_pointer physical_device_list::data() const {
    return m_physical_devices.data();
  }

  physical_device_list::const_iterator physical_device_list::begin() const {
    return m_physical_devices.begin();
  }

  physical_device_list::const_iterator physical_device_list::end() const {
    return m_physical_devices.end();
  }

  physical_device_list::const_iterator physical_device_list::cbegin() const {
    return m_physical_devices.cbegin();
  }

  physical_device_list::const_iterator physical_device_list::cend() const {
    return m_physical_devices.cend();
  }

  physical_device_list::const_reverse_iterator physical_device_list::rbegin() const {
    return m_physical_devices.rbegin();
  }

  physical_device_list::const_reverse_iterator physical_device_list::rend() const {
    return m_physical_devices.rend();
  }

  physical_device_list::const_reverse_iterator physical_device_list::crbegin() const {
    return m_physical_devices.crbegin();
  }

  physical_device_list::const_reverse_iterator physical_device_list::crend() const {
    return m_physical_devices.crend();
  }

  bool physical_device_list::empty() const {
    return m_physical_devices.empty();
  }

  physical_device_list::size_type physical_device_list::size() const {
    return m_physical_devices.size();
  }

}
