#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/adaptors/libvulkan/internal/base/instance_impl.hpp"

#include <cinttypes>

#include "megatech/vulkan/error.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/loader_impl.hpp"

#define VK_DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include "config.hpp"

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  instance_impl::instance_impl(const adaptors::libvulkan::loader& parent_loader, const std::string& application_name,
                               const version& application_version,
                               const std::unordered_set<std::string>& requested_layers) {
    m_parent = parent_loader.share_implementation();
    auto app_info = VkApplicationInfo{ };
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // This conditional is necessary, because VK_VERSION_1_0 doesn't define vkEnumerateInstanceVersion.
#ifdef VK_VERSION_1_1
    {
      auto ver = std::uint32_t{ 0 };
      VK_DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkEnumerateInstanceVersion);
      VK_CHECK(vkEnumerateInstanceVersion(&ver));
      if (version{ ver } < version{ 0, 1, 3, 0 })
      {
        throw error{ "The current Vulkan implementation does not support Vulkan 1.3", VK_ERROR_INITIALIZATION_FAILED };
      }
    }
#else
    throw error{ "The current Vulkan implementation does not support Vulkan 1.3", VK_ERROR_INITIALIZATION_FAILED };
#endif
    app_info.apiVersion = static_cast<std::uint32_t>(version{ 0, 1, 3, 0 });
    app_info.applicationVersion = static_cast<std::uint32_t>(application_version);
    app_info.pApplicationName = application_name.data();
    app_info.engineVersion = VK_MAKE_API_VERSION(0, CONFIG_VULKAN_ENGINE_VERSION_MAJOR,
                                                 CONFIG_VULKAN_ENGINE_VERSION_MINOR,
                                                 CONFIG_VULKAN_ENGINE_VERSION_PATCH);
    app_info.pEngineName = CONFIG_VULKAN_ENGINE_NAME;
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = 0;
    instance_info.ppEnabledExtensionNames = nullptr;
    auto selected_layers = std::vector<const char*>{ };
    for (auto&& layer : requested_layers)
    {
      if (m_parent->available_layers().contains(layer))
      {
        selected_layers.emplace_back(layer.data());
      }
    }
    instance_info.enabledLayerCount = selected_layers.size();
    instance_info.ppEnabledLayerNames = selected_layers.data();
    VK_DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), m_instance });
  }

  instance_impl::~instance_impl() noexcept {
    VK_DECLARE_INSTANCE_PFN_NO_THROW(dispatch_table(), vkDestroyInstance);
    vkDestroyInstance(m_instance, nullptr);
  }

}
