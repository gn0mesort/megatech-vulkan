#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#include <cstring>

#include <bit>
#include <algorithm>
#include <type_traits>

#include <megatech/assertions.hpp>

#include "config.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"

#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

// Allegedly, Microsoft ignores [[no_unique_address]], even in C++20 mode. The MSVC specific attribute shouldn't be
// ignored.
#ifdef CONFIG_COMPILER_MSVC
  #define NO_UNIQUE_ADDRESS_ATTR [[msvc::no_unique_address]]
#else
  #define NO_UNIQUE_ADDRESS_ATTR [[no_unique_address]]
#endif

// All of this stuff is so that I can safely compare and merge Vulkan features in a way that isn't disastrously
// unmaintainable. It is, merely, sort of unmaintainable.
namespace {

  template <typename Type>
  concept vk_extended_feature_type = requires (Type&& t) {
    alignof(Type) >= sizeof(VkBool32);
    { t.sType } -> std::convertible_to<VkStructureType>;
    { t.pNext } -> std::convertible_to<void*>;
  };

  template <std::size_t Padding>
  struct padding final {
    char pad[Padding];
  };

  struct empty final { };

  template <vk_extended_feature_type Type>
  struct padded_feature final {
    VkBool32 value;

    // This is required to ensure that pad is 0 when sizeof(void*) <= 4.
    NO_UNIQUE_ADDRESS_ATTR
    std::conditional_t<(alignof(Type) > sizeof(VkBool32)), padding<alignof(Type) - sizeof(VkBool32)>, empty> pad;
  };

  template <vk_extended_feature_type Type>
  constexpr std::size_t feature_count() {
    return (sizeof(Type) - (alignof(Type) << 1)) / sizeof(padded_feature<Type>);
  }

  template <vk_extended_feature_type Type>
  struct extended_feature_array final {
    VkStructureType sType;
    void* pNext;
    padded_feature<Type> elements[feature_count<Type>()];
  };

  struct basic_feature_array final {
    VkBool32 elements[sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32)];
  };

  // featurecmp works by converting VkPhysicalDevice*Features* into an array-esque type and then doing the
  // comparison iteratively. If a feature is enabled in required but not enabled in actual the test fails.
  template <vk_extended_feature_type Feature>
  bool featurecmp(const Feature& actual, const Feature& required) {
    const auto a_arr = std::bit_cast<extended_feature_array<Feature>>(actual);
    const auto r_arr = std::bit_cast<extended_feature_array<Feature>>(required);
    for (auto i = std::size_t{ 0 }; i < feature_count<Feature>(); ++i)
    {
      if (r_arr.elements[i].value && !a_arr.elements[i].value)
      {
        return false;
      }
    }
    return true;
  }

  bool featurecmp(const VkPhysicalDeviceFeatures& actual, const VkPhysicalDeviceFeatures& required) {
    const auto a_arr = std::bit_cast<basic_feature_array>(actual);
    const auto r_arr = std::bit_cast<basic_feature_array>(required);
    for (auto i = std::size_t{ 0 }; i < (sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32)); ++i)
    {
      if (r_arr.elements[i] && !a_arr.elements[i])
      {
        return false;
      }
    }
    return true;
  }

  // This is similar to featurecmp. Instead of comparing though we merge two sets of features iteratively.
  template <vk_extended_feature_type Feature>
  Feature featuremerge(const Feature& a, const Feature& b) {
    auto a_arr = std::bit_cast<extended_feature_array<Feature>>(a);
    const auto b_arr = std::bit_cast<extended_feature_array<Feature>>(b);
    for (auto i = std::size_t{ 0 }; i < feature_count<Feature>(); ++i) {
      a_arr.elements[i].value = a_arr.elements[i].value || b_arr.elements[i].value;
    }
    return std::bit_cast<Feature>(a_arr);
  }

  VkPhysicalDeviceFeatures featuremerge(const VkPhysicalDeviceFeatures& a, const VkPhysicalDeviceFeatures& b) {
    auto a_arr = std::bit_cast<basic_feature_array>(a);
    const auto b_arr = std::bit_cast<basic_feature_array>(b);
    for (auto i = std::size_t{ 0 }; i < (sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32)); ++i) {
      a_arr.elements[i] = a_arr.elements[i] || b_arr.elements[i];
    }
    return std::bit_cast<VkPhysicalDeviceFeatures>(a_arr);
  }

}

namespace megatech::vulkan::internal::base {

  void physical_device_description_impl::set_queue_families(const std::int64_t primary, const std::int64_t compute,
                                                            const std::int64_t transfer) {
    if (primary > static_cast<std::int64_t>(m_queue_family_properties.size()) ||
        compute > static_cast<std::int64_t>(m_queue_family_properties.size()) ||
        transfer > static_cast<std::int64_t>(m_queue_family_properties.size()))
    {
      throw error{ "You cannot assign a queue family index that is out of range." };
    }
    m_primary_queue_family = primary;
    m_async_compute_queue_family = compute;
    m_async_transfer_queue_family = transfer;
    MEGATECH_POSTCONDITION(m_primary_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    MEGATECH_POSTCONDITION(m_async_compute_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    MEGATECH_POSTCONDITION(m_async_transfer_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
  }

  void physical_device_description_impl::add_required_extension(const std::string& extension) {
    if (extension.empty())
    {
      return;
    }
    m_required_extensions.insert(extension);
  }

  void physical_device_description_impl::require_1_0_features(const VkPhysicalDeviceFeatures& features) {
    m_required_features.features = featuremerge(features, m_required_features.features);
  }

  void physical_device_description_impl::require_1_1_features(const VkPhysicalDeviceVulkan11Features& features) {
    m_required_features_1_1 = featuremerge(features, m_required_features_1_1);
  }

  void physical_device_description_impl::require_1_2_features(const VkPhysicalDeviceVulkan12Features& features) {
    m_required_features_1_2 = featuremerge(features, m_required_features_1_2);
  }

  void physical_device_description_impl::require_1_3_features(const VkPhysicalDeviceVulkan13Features& features) {
    m_required_features_1_3 = featuremerge(features, m_required_features_1_3);
    MEGATECH_POSTCONDITION(m_required_features_1_3.dynamicRendering == VK_TRUE);
  }

  void physical_device_description_impl::append_extended_feature_chain(void *const next) {
    m_dynamic_rendering_local_read_features.pNext = next;
  }

  bool physical_device_description_impl::has_extended_features() const {
    return true;
  }

  physical_device_description_impl::physical_device_description_impl(std::shared_ptr<const parent_type> parent,
                                                                     VkPhysicalDevice handle) :
  m_parent{ parent },
  m_handle{ handle } {
    if (!parent)
    {
      throw error{ "The parent instance cannot be null." };
    }
    if (handle == VK_NULL_HANDLE)
    {
      throw error{ "The physical device handle cannot be null." };
    }
    auto properties2 = VkPhysicalDeviceProperties2{ };
    properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    m_properties_1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES;
    properties2.pNext = &m_properties_1_1;
    m_properties_1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES;
    m_properties_1_1.pNext = &m_properties_1_2;
    m_properties_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES;
    m_properties_1_2.pNext = &m_properties_1_3;
    m_properties_1_3.pNext = nullptr;
    DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkGetPhysicalDeviceProperties2);
    vkGetPhysicalDeviceProperties2(m_handle, &properties2);
    m_properties_1_3.pNext = nullptr;
    m_properties_1_2.pNext = nullptr;
    m_properties_1_1.pNext = nullptr;
    m_properties_1_0 = properties2.properties;
    auto features2 = VkPhysicalDeviceFeatures2{ };
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &m_features_1_1;
    m_features_1_1.pNext = &m_features_1_2;
    m_features_1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    m_features_1_2.pNext = &m_features_1_3;
    m_features_1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    m_features_1_3.pNext = &m_dynamic_rendering_local_read_features;
    m_features_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    m_dynamic_rendering_local_read_features.pNext = nullptr;
    m_dynamic_rendering_local_read_features.sType =
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
    DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkGetPhysicalDeviceFeatures2);
    vkGetPhysicalDeviceFeatures2(m_handle, &features2);
    m_features_1_3.pNext = nullptr;
    m_features_1_2.pNext = nullptr;
    m_features_1_1.pNext = nullptr;
    m_features_1_0 = features2.features;
    m_required_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    m_required_features.pNext = &m_required_features_1_1;
    m_required_features_1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    m_required_features_1_1.pNext = &m_required_features_1_2;
    m_required_features_1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    m_required_features_1_2.pNext = &m_required_features_1_3;
    m_required_features_1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    m_required_features_1_3.pNext = &m_required_dynamic_rendering_local_read_features;
    m_required_features_1_3.dynamicRendering = VK_TRUE;
    m_required_dynamic_rendering_local_read_features.sType =
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
    m_required_dynamic_rendering_local_read_features.pNext = nullptr;
    m_required_dynamic_rendering_local_read_features.dynamicRenderingLocalRead = true;
    DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkEnumerateDeviceExtensionProperties);
    {
      auto sz = std::uint32_t{ 0 };
      VK_CHECK(vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &sz, nullptr));
      auto properties = std::vector<VkExtensionProperties>(sz);
      VK_CHECK(vkEnumerateDeviceExtensionProperties(m_handle, nullptr, &sz, properties.data()));
      for (const auto& property : properties)
      {
        m_available_extensions.insert(property.extensionName);
      }
      for (const auto& layer : m_parent->enabled_layers())
      {
        VK_CHECK(vkEnumerateDeviceExtensionProperties(m_handle, layer.data(), &sz, nullptr));
        properties.resize(sz);
        VK_CHECK(vkEnumerateDeviceExtensionProperties(m_handle, layer.data(), &sz, properties.data()));
        for (const auto& property : properties)
        {
          m_available_extensions.insert(property.extensionName);
        }
      }
    }
    DECLARE_INSTANCE_PFN(m_parent->dispatch_table(), vkGetPhysicalDeviceQueueFamilyProperties);
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
          const auto has_compute =
            static_cast<std::int64_t>(m_queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT);
          const auto has_transfer_only =
            static_cast<std::int64_t>(!(m_queue_family_properties[i].queueFlags &
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
    MEGATECH_POSTCONDITION(m_parent != nullptr);
    MEGATECH_POSTCONDITION(m_parent == parent);
    MEGATECH_POSTCONDITION(m_handle != VK_NULL_HANDLE);
    MEGATECH_POSTCONDITION(m_handle == handle);
    MEGATECH_POSTCONDITION(m_primary_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    MEGATECH_POSTCONDITION(m_async_compute_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    MEGATECH_POSTCONDITION(m_async_transfer_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    MEGATECH_POSTCONDITION(m_required_extensions.contains("VK_KHR_dynamic_rendering_local_read"));
    MEGATECH_POSTCONDITION(m_required_features.pNext == &m_required_features_1_1);
    MEGATECH_POSTCONDITION(m_required_features.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);
    MEGATECH_POSTCONDITION(m_required_features_1_1.pNext == &m_required_features_1_2);
    MEGATECH_POSTCONDITION(m_required_features_1_1.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
    MEGATECH_POSTCONDITION(m_required_features_1_2.pNext == &m_required_features_1_3);
    MEGATECH_POSTCONDITION(m_required_features_1_2.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
    MEGATECH_POSTCONDITION(m_required_features_1_3.pNext == &m_required_dynamic_rendering_local_read_features);
    MEGATECH_POSTCONDITION(m_required_features_1_3.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);
    MEGATECH_POSTCONDITION(m_required_features_1_3.dynamicRendering == VK_TRUE);
    MEGATECH_POSTCONDITION(m_required_dynamic_rendering_local_read_features.pNext == nullptr);
    MEGATECH_POSTCONDITION(m_required_dynamic_rendering_local_read_features.sType ==
                           VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR);
    MEGATECH_POSTCONDITION(m_required_dynamic_rendering_local_read_features.dynamicRenderingLocalRead == VK_TRUE);
  }

  physical_device_description_impl::handle_type physical_device_description_impl::handle() const {
    MEGATECH_PRECONDITION(m_handle != VK_NULL_HANDLE);
    return m_handle;
  }

  const physical_device_description_impl::parent_type& physical_device_description_impl::parent() const {
    MEGATECH_PRECONDITION(m_parent != nullptr);
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

  const VkPhysicalDeviceFeatures& physical_device_description_impl::features_1_0() const {
    return m_features_1_0;
  }

  const VkPhysicalDeviceVulkan11Features& physical_device_description_impl::features_1_1() const {
    return m_features_1_1;
  }

  const VkPhysicalDeviceVulkan12Features& physical_device_description_impl::features_1_2() const {
    return m_features_1_2;
  }

  const VkPhysicalDeviceVulkan13Features& physical_device_description_impl::features_1_3() const {
    return m_features_1_3;
  }

  const std::unordered_set<std::string>& physical_device_description_impl::available_extensions() const {
    return m_available_extensions;
  }

  const std::vector<VkQueueFamilyProperties>& physical_device_description_impl::queue_family_properties() const {
    return m_queue_family_properties;
  }

  int64_t physical_device_description_impl::primary_queue_family_index() const {
    MEGATECH_PRECONDITION(m_primary_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    return m_primary_queue_family;
  }

  int64_t physical_device_description_impl::async_compute_queue_family_index() const {
    MEGATECH_PRECONDITION(m_async_compute_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    return m_async_compute_queue_family;
  }

  int64_t physical_device_description_impl::async_transfer_queue_family_index() const {
    MEGATECH_PRECONDITION(m_async_transfer_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    return m_async_transfer_queue_family;
  }

  const VkQueueFamilyProperties& physical_device_description_impl::primary_queue_family_properties() const {
    MEGATECH_PRECONDITION(m_primary_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    if (m_primary_queue_family == -1)
    {
      throw error{ "The physical device does not support the minimum required operations." };
    }
    return m_queue_family_properties[m_primary_queue_family];
  }

  const VkQueueFamilyProperties& physical_device_description_impl::async_compute_queue_family_properties() const {
    MEGATECH_PRECONDITION(m_async_compute_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    if (m_async_compute_queue_family == -1)
    {
      throw error{ "The physical device does not support asynchronous compute operations." };
    }
    return m_queue_family_properties[m_async_compute_queue_family];
  }

  const VkQueueFamilyProperties& physical_device_description_impl::async_transfer_queue_family_properties() const {
    MEGATECH_PRECONDITION(m_async_transfer_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    if (m_async_transfer_queue_family == -1)
    {
      throw error{ "The physical device does not support asynchronous transfer operations." };
    }
    return m_queue_family_properties[m_async_transfer_queue_family];
  }

  bool physical_device_description_impl::is_valid() const {
    MEGATECH_PRECONDITION(m_primary_queue_family < static_cast<std::int64_t>(m_queue_family_properties.size()));
    auto has_extensions = true;
    for (const auto& extension : m_required_extensions)
    {
      has_extensions = has_extensions && m_available_extensions.contains(extension);
    }
    return m_primary_queue_family != -1 &&
           has_extensions &&
           version{ m_properties_1_0.apiVersion } >= MINIMUM_VULKAN_VERSION &&
           featurecmp(m_features_1_0, m_required_features.features) &&
           featurecmp(m_features_1_1, m_required_features_1_1) &&
           featurecmp(m_features_1_2, m_required_features_1_2) &&
           featurecmp(m_features_1_3, m_required_features_1_3) &&
           featurecmp(m_dynamic_rendering_local_read_features, m_required_dynamic_rendering_local_read_features) &&
           has_extended_features();
  }

  const std::unordered_set<std::string>& physical_device_description_impl::required_extensions() const {
    return m_required_extensions;
  }

  const VkPhysicalDeviceFeatures2& physical_device_description_impl::required_features() const {
    MEGATECH_PRECONDITION(m_required_extensions.contains("VK_KHR_dynamic_rendering_local_read"));
    MEGATECH_PRECONDITION(m_required_features.pNext == &m_required_features_1_1);
    MEGATECH_PRECONDITION(m_required_features.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2);
    MEGATECH_PRECONDITION(m_required_features_1_1.pNext == &m_required_features_1_2);
    MEGATECH_PRECONDITION(m_required_features_1_1.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES);
    MEGATECH_PRECONDITION(m_required_features_1_2.pNext == &m_required_features_1_3);
    MEGATECH_PRECONDITION(m_required_features_1_2.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES);
    MEGATECH_PRECONDITION(m_required_features_1_3.pNext == &m_required_dynamic_rendering_local_read_features);
    MEGATECH_PRECONDITION(m_required_features_1_3.sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES);
    MEGATECH_PRECONDITION(m_required_features_1_3.dynamicRendering == VK_TRUE);
    MEGATECH_PRECONDITION(m_required_dynamic_rendering_local_read_features.pNext == nullptr);
    MEGATECH_PRECONDITION(m_required_dynamic_rendering_local_read_features.sType ==
                          VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR);
    MEGATECH_PRECONDITION(m_required_dynamic_rendering_local_read_features.dynamicRenderingLocalRead == VK_TRUE);
    return m_required_features;
  }

}
