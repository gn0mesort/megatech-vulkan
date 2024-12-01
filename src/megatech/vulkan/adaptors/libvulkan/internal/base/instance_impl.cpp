#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/adaptors/libvulkan/internal/base/instance_impl.hpp"

#include <cinttypes>

#include "megatech/vulkan/error.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/loader_impl.hpp"

#define VK_DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define VK_DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include "config.hpp"

#include <iostream>

namespace {

  VkBool32 vkDebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT,
                                            const VkDebugUtilsMessengerCallbackDataEXT* data, void*) {
    std::clog << data->pMessage << std::endl;
    return VK_FALSE;
  }

}

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
        m_enabled_layers.insert(layer);
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


  debug_instance_impl::debug_instance_impl(const adaptors::libvulkan::loader& parent_loader,
                                           const std::string& application_name, const version& application_version,
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
        m_enabled_layers.insert(layer);
      }
    }
    auto available_extensions = std::unordered_set<std::string>{ };
    {
      auto sz = std::uint32_t{ 0 };
      auto extension_properties = std::vector<VkExtensionProperties>{ };
      VK_DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkEnumerateInstanceExtensionProperties);
      VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &sz, nullptr));
      extension_properties.resize(sz);
      VK_CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &sz, extension_properties.data()));
      for (auto&& extension_property : extension_properties)
      {
        available_extensions.insert(extension_property.extensionName);
      }
      for (auto&& layer : selected_layers)
      {
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layer, &sz, nullptr));
        extension_properties.resize(sz);
        VK_CHECK(vkEnumerateInstanceExtensionProperties(layer, &sz, extension_properties.data()));
        for (auto&& extension_property : extension_properties)
        {
          available_extensions.insert(extension_property.extensionName);
        }
      }
    }
    if (!available_extensions.contains("VK_EXT_debug_utils"))
    {
      throw error{ "The current Vulkan implementation doesn't support debug output.", VK_ERROR_EXTENSION_NOT_PRESENT };
    }
    auto selected_extensions = std::vector<const char*>{ "VK_EXT_debug_utils" };
    auto debug_info = VkDebugUtilsMessengerCreateInfoEXT{ };
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = vkDebugUtilsMessengerCallbackEXT;
    // For the sake of safety, I'm using the 32-bit Vulkan constants here and not true/false.
    auto setting_validate_sync = VK_TRUE;
    auto setting_validate_best_practices = VK_TRUE;
    const auto layer_settings = std::array<VkLayerSettingEXT, 2>{
      VkLayerSettingEXT{ "VK_LAYER_KHRONOS_validation", "validate_sync", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1,
                         &setting_validate_sync },
      VkLayerSettingEXT{ "VK_LAYER_KHRONOS_validation", "validate_best_practices", VK_LAYER_SETTING_TYPE_BOOL32_EXT, 1,
                         &setting_validate_best_practices }
    };
    auto layer_settings_info = VkLayerSettingsCreateInfoEXT{ };
    layer_settings_info.sType = VK_STRUCTURE_TYPE_LAYER_SETTINGS_CREATE_INFO_EXT;
    layer_settings_info.settingCount = layer_settings.size();
    layer_settings_info.pSettings = layer_settings.data();
    if (available_extensions.contains("VK_EXT_layer_settings"))
    {
      selected_extensions.emplace_back("VK_EXT_layer_settings");
      instance_info.pNext = &layer_settings_info;
      layer_settings_info.pNext = &debug_info;
    }
    else
    {
      instance_info.pNext = &debug_info;
    }
    instance_info.enabledLayerCount = selected_layers.size();
    instance_info.ppEnabledLayerNames = selected_layers.data();
    instance_info.enabledExtensionCount = selected_extensions.size();
    instance_info.ppEnabledExtensionNames = selected_extensions.data();
    VK_DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    for (auto&& extension : selected_extensions)
    {
      m_enabled_extensions.insert(extension);
    }
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), m_instance });
    VK_DECLARE_INSTANCE_PFN(dispatch_table(), vkCreateDebugUtilsMessengerEXT);
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_instance, &debug_info, nullptr, &m_debug_messenger));
  }


  debug_instance_impl::~debug_instance_impl() noexcept {
    VK_DECLARE_INSTANCE_PFN_NO_THROW(dispatch_table(), vkDestroyDebugUtilsMessengerEXT);
    vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
    VK_DECLARE_INSTANCE_PFN_NO_THROW(dispatch_table(), vkDestroyInstance);
    vkDestroyInstance(m_instance, nullptr);
  }

}
