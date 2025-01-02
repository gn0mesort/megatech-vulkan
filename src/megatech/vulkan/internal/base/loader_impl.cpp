#include "megatech/vulkan/internal/base/loader_impl.hpp"

#include <vector>

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/layer_description_proxy.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace megatech::vulkan::internal::base {

  loader_impl::loader_impl(const loader_function_builder& builder) :
  m_gdt{ new dispatch::global::table{ builder.build() } } {
    DECLARE_GLOBAL_PFN(*m_gdt, vkEnumerateInstanceLayerProperties);
    auto sz = std::uint32_t{ };
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, nullptr));
    auto properties = std::vector<VkLayerProperties>(sz);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, properties.data()));
    for (const auto& property : properties)
    {
      m_available_layers.emplace(layer_description_proxy{ property });
    }
  }

  const dispatch::global::table& loader_impl::dispatch_table() const {
    return *m_gdt;
  }

  const std::unordered_set<layer_description>& loader_impl::available_layers() const {
    return m_available_layers;
  }

}
