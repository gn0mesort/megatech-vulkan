#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_impl.hpp"

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

#include <cinttypes>

#include <utility>

#include <megatech/vulkan/dispatch/tables.hpp>

#include <megatech/vulkan/internal/tag.hpp>
#include <megatech/vulkan/internal/base/vulkandefs.hpp>

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  loader_impl::loader_impl() {
    m_gdt.reset(new megatech::vulkan::dispatch::global::table{ vkGetInstanceProcAddr });
    DECLARE_GLOBAL_PFN(*m_gdt, vkEnumerateInstanceLayerProperties);
    auto sz = std::uint32_t{ };
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, nullptr));
    auto properties = std::vector<VkLayerProperties>(sz);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, properties.data()));
    for (const auto& property : properties)
    {
      m_available_layers.emplace_back(property, megatech::vulkan::internal::tag{ });
    }
  }

  loader_impl::loader_impl(const loader_impl& other) {
    m_gdt.reset(new megatech::vulkan::dispatch::global::table{ *other.m_gdt });
    m_available_layers = other.m_available_layers;
  }

  loader_impl& loader_impl::operator=(const loader_impl& rhs) {
    if (this != &rhs)
    {
      auto cpy = loader_impl{ rhs };
      std::swap(m_gdt, cpy.m_gdt);
      std::swap(m_available_layers, cpy.m_available_layers);
    }
    return *this;
  }

}
