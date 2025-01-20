#include "megatech/vulkan/internal/base/loader_impl.hpp"

#include <vector>

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/layer_description_proxy.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  loader_impl::loader_impl(const PFN_vkGetInstanceProcAddr gipa) :
  m_gdt{ new dispatch::global::table{ gipa } } {
    auto sz = std::uint32_t{ };
    {
      DECLARE_GLOBAL_PFN(*m_gdt, vkEnumerateInstanceLayerProperties);
      VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, nullptr));
      auto properties = std::vector<VkLayerProperties>(sz);
      VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, properties.data()));
      for (const auto& property : properties)
      {
        m_available_layers.emplace(layer_description_proxy{ property });
      }
    }
    {
      DECLARE_GLOBAL_PFN(*m_gdt, vkEnumerateInstanceExtensionProperties);
      VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &sz, nullptr));
      auto properties = std::vector<VkExtensionProperties>(sz);
      VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &sz, properties.data()));
      m_available_extensions[""] = { };
      for (const auto& property : properties)
      {
        m_available_extensions[""].insert(property.extensionName);
      }
      for (const auto& layer : m_available_layers)
      {
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layer.name().data(), &sz, nullptr));
        properties.resize(sz);
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layer.name().data(), &sz, properties.data()));
        m_available_extensions[layer.name()] = { };
        for (const auto& property : properties)
        {
          m_available_extensions[layer.name()].insert(property.extensionName);
        }
      }
    }
    m_available_extensions.rehash(0);
  }

  const dispatch::global::table& loader_impl::dispatch_table() const {
    return *m_gdt;
  }

  const std::unordered_set<layer_description>& loader_impl::available_layers() const {
    return m_available_layers;
  }

  const std::unordered_set<std::string>& loader_impl::available_instance_extensions() const {
    return available_instance_extensions("");
  }

  const std::unordered_set<std::string>& loader_impl::available_instance_extensions(const std::string& layer) const {
    return m_available_extensions.at(layer);
  }

}
