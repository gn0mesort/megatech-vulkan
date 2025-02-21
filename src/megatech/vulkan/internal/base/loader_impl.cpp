/**
 * @file loader_impl.cpp
 * @brief Loader Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/internal/base/loader_impl.hpp"

#include <vector>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/layer_description_proxy.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  loader_impl::loader_impl(const PFN_vkGetInstanceProcAddr pfn) {
    set_loader_pfn(pfn);
  }

  void loader_impl::set_loader_pfn(const PFN_vkGetInstanceProcAddr pfn) {
    MEGATECH_PRECONDITION(pfn != nullptr);
    MEGATECH_PRECONDITION(m_gdt == nullptr);
    if (m_gdt)
    {
      throw error{ "The loader cannot be reset once initialized." };
    }
    if (!pfn)
    {
      throw error{ "The loader function cannot be null." };
    }
    m_gdt.reset(new dispatch::global::table{ pfn });
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
    MEGATECH_POSTCONDITION(m_gdt != nullptr);
    MEGATECH_POSTCONDITION(m_available_extensions.contains(""));
  }

  const dispatch::global::table& loader_impl::dispatch_table() const {
    MEGATECH_PRECONDITION(m_gdt != nullptr);
    return *m_gdt;
  }

  const std::unordered_set<layer_description>& loader_impl::available_layers() const {
    MEGATECH_PRECONDITION(m_available_extensions.contains(""));
    return m_available_layers;
  }

  const std::unordered_set<std::string>& loader_impl::available_instance_extensions() const {
    MEGATECH_PRECONDITION(m_available_extensions.contains(""));
    return available_instance_extensions("");
  }

  const std::unordered_set<std::string>& loader_impl::available_instance_extensions(const std::string& layer) const {
    MEGATECH_PRECONDITION(m_available_extensions.contains(""));
    if (!layer.empty() && !m_available_layers.contains(layer))
    {
      throw error{ "Extensions can only be queried for layers that are available to the loader." };
    }
    return m_available_extensions.at(layer);
  }

  instance_impl* loader_impl::resolve_instance(const application_description& app_description,
                                               const std::unordered_set<std::string>& requested_layers) const {
    MEGATECH_PRECONDITION(m_gdt != nullptr);
    auto weak = weak_from_this();
    if (weak.expired())
    {
      throw error{ "The loader implementation isn't managed by a shared pointer." };
    }
    return new instance_impl{ weak.lock(), app_description, requested_layers };
  }

  debug_instance_impl*
  loader_impl::resolve_debug_instance(const application_description& app_description,
                                      const std::unordered_set<std::string>& requested_layers) const {
    MEGATECH_PRECONDITION(m_gdt != nullptr);
    auto weak = weak_from_this();
    if (weak.expired())
    {
      throw error{ "The loader implementation isn't managed by a shared pointer." };
    }
    return new debug_instance_impl{ weak.lock(), app_description, requested_layers };
  }


  debug_instance_impl*
  loader_impl::resolve_debug_instance(const application_description& app_description,
                                      const debug_messenger_description& messenger_description,
                                     const std::unordered_set<std::string>& requested_layers) const {
    MEGATECH_PRECONDITION(m_gdt != nullptr);
    auto weak = weak_from_this();
    if (weak.expired())
    {
      throw error{ "The loader implementation isn't managed by a shared pointer." };
    }
    return new debug_instance_impl{ weak.lock(), app_description, messenger_description, requested_layers };
  }

}
