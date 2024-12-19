#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/physical_devices.hpp"

#include <cinttypes>

#include "megatech/vulkan/instance.hpp"
#include "megatech/vulkan/version.hpp"

#include "megatech/vulkan/internal/tag.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#define VK_DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  void physical_device_description_impl_dtor::operator()(physical_device_description_impl* p) const noexcept {
    delete p;
  }

}

namespace megatech::vulkan {

  physical_device_description::physical_device_description(std::shared_ptr<const internal::base::instance_impl> parent,
                                                           VkPhysicalDevice handle, const internal::tag&) :
  m_impl{ new internal::base::physical_device_description_impl{ parent, handle },
          internal::base::physical_device_description_impl_dtor{ } } { }

  bool physical_device_description::operator==(const physical_device_description& rhs) const noexcept {
    return m_impl->handle() == rhs.implementation().handle();
  }

  const physical_device_description::implementation_type& physical_device_description::implementation() const {
    return *m_impl;
  }

  physical_device_description::implementation_type& physical_device_description::implementation() {
    return *m_impl;
  }

  std::shared_ptr<const physical_device_description::implementation_type> physical_device_description::share_implementation() const {
    return m_impl;
  }

  bool physical_device_description::supports_rendering() const {
    return m_impl->primary_queue_family_properties().queueFlags & VK_QUEUE_GRAPHICS_BIT;
  }

  bool physical_device_description::supports_presentation(const surface&) const {
    return supports_rendering() /* && TODO: check surface support */;
  }

  bool physical_device_description::supports_async_execution() const {
    return m_impl->async_transfer_queue_family_index() != -1;
  }

  int physical_device_description::supports_async_transfer() const {
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
  m_physical_devices{ std::move(filtered_list) } { }

  bool physical_device_list::is_valid(const physical_device_description& physical_device) const {
    const auto& impl = physical_device.implementation();
    const auto good_version = version{ impl.properties_1_0().apiVersion } >= version{ 0, 1, 3, 0 };
    const auto has_primary_queue = impl.primary_queue_family_index() != -1;
    VK_DECLARE_INSTANCE_PFN(impl.parent().dispatch_table(), vkGetPhysicalDeviceFeatures2);
    auto features2 = VkPhysicalDeviceFeatures2{ };
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    auto local_read_features = VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR{ };
    local_read_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
    features2.pNext = &local_read_features;
    vkGetPhysicalDeviceFeatures2(impl.handle(), &features2);
    const auto has_dynamic_rendering = impl.features_1_3().dynamicRendering &&
                                       local_read_features.dynamicRenderingLocalRead &&
                                       impl.available_extensions().contains("VK_KHR_dyanmic_rendering_local_read");
    return good_version && has_primary_queue && has_dynamic_rendering;
  }

  physical_device_list::physical_device_list(const instance& inst) {
    auto parent = inst.share_implementation();
    VK_DECLARE_INSTANCE_PFN(parent->dispatch_table(), vkEnumeratePhysicalDevices);
    auto sz = std::uint32_t{ 0 };
    VK_CHECK(vkEnumeratePhysicalDevices(parent->handle(), &sz, nullptr));
    auto handles = std::vector<VkPhysicalDevice>(sz);
    VK_CHECK(vkEnumeratePhysicalDevices(parent->handle(), &sz, handles.data()));
    m_physical_devices.reserve(sz);
    for (auto&& handle : handles)
    {
      auto tmp = physical_device_description{ parent, handle, internal::tag{ } };
      if (is_valid(tmp) && parent->is_valid(tmp))
      {
        m_physical_devices.emplace_back(tmp);
      }
    }
    m_physical_devices.shrink_to_fit();
  }

  physical_device_list::const_reference physical_device_list::operator[](const physical_device_list::size_type index) const {
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
    return m_physical_devices.at(index);
  }

  physical_device_list::const_reference physical_device_list::front() const {
    return m_physical_devices.front();
  }

  physical_device_list::const_reference physical_device_list::back() const {
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
