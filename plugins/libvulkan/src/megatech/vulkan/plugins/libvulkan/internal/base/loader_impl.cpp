#include "megatech/vulkan/plugins/libvulkan/internal/base/loader_impl.hpp"

#include <utility>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

#include <megatech/vulkan/dispatch/tables.hpp>

namespace megatech::vulkan::plugins::libvulkan::internal::base {

  loader_impl::loader_impl() {
    m_gdt.reset(new megatech::vulkan::dispatch::global::table{ vkGetInstanceProcAddr });
  }

  loader_impl::loader_impl(const loader_impl& other) {
    m_gdt.reset(new megatech::vulkan::dispatch::global::table{ *other.m_gdt });
  }

  loader_impl& loader_impl::operator=(const loader_impl& rhs) {
    if (this != &rhs)
    {
      auto cpy = loader_impl{ rhs };
      std::swap(m_gdt, cpy.m_gdt);
    }
    return *this;
  }

}
