#include "megatech/vulkan/internal/base/instance_impl.hpp"

#include <utility>
#include <algorithm>

#include "config.hpp"

#include "megatech/vulkan/application_description.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd)
#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace {

  // Not a very meritorious implementation. This is really just a stub for my sanity right now.
  VKAPI_ATTR VkBool32 VKAPI_CALL
  vkDebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                   VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                   void* pUserData) {
    using megatech::vulkan::debug_messenger_description;
    using megatech::vulkan::bitmask;
    if (pUserData)
    {
      const auto& sink = *reinterpret_cast<std::function<debug_messenger_description::message_sink_fn>*>(pUserData);
      sink(static_cast<bitmask>(messageTypes), static_cast<bitmask>(messageSeverity),pCallbackData->pMessage);
    }
    return VK_FALSE;
  }

}

namespace megatech::vulkan::internal::base {

  instance_description::instance_description(const std::unordered_set<std::string>& requested_layers,
                                             const std::unordered_set<std::string>& requested_extensions,
                                             const std::unordered_set<std::string>& required_extensions) :
  m_requested_layers{ requested_layers },
  m_requested_extensions{ requested_extensions },
  m_required_extensions{ required_extensions } { }

  const std::unordered_set<std::string>& instance_description::requested_layers() const {
    return m_requested_layers;
  }

  const std::unordered_set<std::string>& instance_description::requested_extensions() const {
    return m_requested_extensions;
  }

  const std::unordered_set<std::string>& instance_description::required_extensions() const {
    return m_required_extensions;
  }

  instance_impl::instance_impl(const std::shared_ptr<const loader_impl>& parent,
                               std::unique_ptr<const class physical_device_allocator>&& allocator) :
  m_physical_device_allocator{ std::move(allocator) },
  m_parent{ parent } { }

  instance_impl::instance_impl(const std::shared_ptr<const loader_impl>& parent,
                               std::unique_ptr<const class physical_device_allocator>&& allocator,
                               const application_description& app_description,
                               const instance_description& description) :
  m_physical_device_allocator{ std::move(allocator) },
  m_parent{ parent } {
    {
      DECLARE_GLOBAL_PFN_NO_THROW(parent->dispatch_table(), vkEnumerateInstanceVersion);
      if (!vkEnumerateInstanceVersion)
      {
        throw error{ "Vulkan instances must support Vulkan >=1.3." };
      }
      auto ver = std::uint32_t{ 0 };
      vkEnumerateInstanceVersion(&ver);
      if (version{ ver } < MINIMUM_VULKAN_VERSION)
      {
        throw error{ "Vulkan instances must support Vulkan >=1.3." };
      }
    }
    auto application_info = VkApplicationInfo{ };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.applicationVersion = static_cast<std::uint32_t>(app_description.version());
    application_info.pApplicationName = app_description.name().data();
    application_info.apiVersion = static_cast<std::uint32_t>(MINIMUM_VULKAN_VERSION);
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &application_info;
    for (const auto& layer : description.requested_layers())
    {
      if (parent->available_layers().contains(layer))
      {
        m_enabled_layers.insert(layer);
      }
    }
    auto enabled_layers = std::vector<const char*>{ };
    for (const auto& layer : m_enabled_layers)
    {
      enabled_layers.emplace_back(layer.data());
    }
    instance_info.enabledLayerCount = enabled_layers.size();
    instance_info.ppEnabledLayerNames = enabled_layers.data();
    for (const auto& extension : description.required_extensions())
    {
      m_enabled_extensions.insert(extension);
    }
    for (const auto& extension : description.requested_extensions())
    {
      auto valid = parent->available_instance_extensions().contains(extension);
      for (const auto& layer : m_enabled_layers)
      {
        valid |= parent->available_instance_extensions(layer).contains(extension);
      }
      if (valid)
      {
        m_enabled_extensions.insert(extension);
      }
    }
    auto enabled_extensions = std::vector<const char*>{ };
    for (const auto& extension : m_enabled_extensions)
    {
      enabled_extensions.emplace_back(extension.data());
    }
    instance_info.enabledExtensionCount = enabled_extensions.size();
    instance_info.ppEnabledExtensionNames = enabled_extensions.data();
    DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), m_instance });
  }

  instance_impl::~instance_impl() noexcept {
    // This allows changing destruction behavior in a derived class (e.g., debug_instance_impl). A derived class
    // can destroy the instance on its own and then null the handle to prevent a double free situation.
    if (m_instance != VK_NULL_HANDLE)
    {
      DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyInstance);
      vkDestroyInstance(m_instance, nullptr);
      m_instance = VK_NULL_HANDLE;
    }
  }

  const class physical_device_allocator& instance_impl::physical_device_allocator() const {
    return *m_physical_device_allocator;
  }

  const dispatch::instance::table& instance_impl::dispatch_table() const {
    return *m_idt;
  }

  instance_impl::handle_type instance_impl::handle() const {
    return m_instance;
  }

  const instance_impl::parent_type& instance_impl::parent() const {
    return *m_parent;
  }

  const std::unordered_set<std::string>& instance_impl::enabled_layers() const {
    return m_enabled_layers;
  }

  const std::unordered_set<std::string>& instance_impl::enabled_extensions() const {
    return m_enabled_extensions;
  }

  debug_instance_impl::debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                                           std::unique_ptr<const class physical_device_allocator>&& allocator,
                                           const application_description& app_description,
                                           const instance_description& description) :
  instance_impl{ parent, std::move(allocator), app_description, description } { }

  debug_instance_impl::debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                                           std::unique_ptr<const class physical_device_allocator>&& allocator,
                                           const application_description& app_description,
                                           const debug_messenger_description& messenger_description,
                                           const instance_description& description) :
  instance_impl{ parent, std::move(allocator) },
  m_message_sink{ messenger_description.sink() } {
    {
      DECLARE_GLOBAL_PFN_NO_THROW(parent->dispatch_table(), vkEnumerateInstanceVersion);
      if (!vkEnumerateInstanceVersion)
      {
        throw error{ "Vulkan instances must support Vulkan >=1.3." };
      }
      auto ver = std::uint32_t{ 0 };
      vkEnumerateInstanceVersion(&ver);
      if (version{ ver } < MINIMUM_VULKAN_VERSION)
      {
        throw error{ "Vulkan instances must support Vulkan >=1.3." };
      }
    }
    auto application_info = VkApplicationInfo{ };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.applicationVersion = static_cast<std::uint32_t>(app_description.version());
    application_info.pApplicationName = app_description.name().data();
    application_info.apiVersion = static_cast<std::uint32_t>(MINIMUM_VULKAN_VERSION);
    auto debug_utils_messenger_info = VkDebugUtilsMessengerCreateInfoEXT{ };
    debug_utils_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_utils_messenger_info.messageSeverity =
      static_cast<VkDebugUtilsMessageSeverityFlagsEXT>(messenger_description.accepted_message_severities());
    debug_utils_messenger_info.messageType =
      static_cast<VkDebugUtilsMessageTypeFlagsEXT>(messenger_description.accepted_message_types());
    debug_utils_messenger_info.pfnUserCallback = vkDebugUtilsMessengerCallbackEXT;
    debug_utils_messenger_info.pUserData = &m_message_sink;
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &application_info;
    for (const auto& layer : description.requested_layers())
    {
      if (parent->available_layers().contains(layer))
      {
        m_enabled_layers.insert(layer);
      }
    }
    auto enabled_layers = std::vector<const char*>{ };
    for (const auto& layer : m_enabled_layers)
    {
      enabled_layers.emplace_back(layer.data());
    }
    instance_info.enabledLayerCount = enabled_layers.size();
    instance_info.ppEnabledLayerNames = enabled_layers.data();
    for (const auto& extension : description.required_extensions())
    {
      m_enabled_extensions.insert(extension);
    }
    for (const auto& extension : description.requested_extensions())
    {
      auto valid = parent->available_instance_extensions().contains(extension);
      for (const auto& layer : m_enabled_layers)
      {
        valid |= parent->available_instance_extensions(layer).contains(extension);
      }
      if (valid)
      {
        m_enabled_extensions.insert(extension);
      }
    }
    auto enabled_extensions = std::vector<const char*>{ };
    for (const auto& extension : m_enabled_extensions)
    {
      enabled_extensions.emplace_back(extension.data());
    }
    instance_info.enabledExtensionCount = enabled_extensions.size();
    instance_info.ppEnabledExtensionNames = enabled_extensions.data();
    instance_info.pNext = &debug_utils_messenger_info;
    DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), m_instance });
    DECLARE_INSTANCE_PFN(*m_idt, vkCreateDebugUtilsMessengerEXT);
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_instance, &debug_utils_messenger_info, nullptr,
                                            &m_debug_utils_messenger));
  }

  debug_instance_impl::~debug_instance_impl() noexcept {
    if (m_debug_utils_messenger != VK_NULL_HANDLE)
    {
      DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyDebugUtilsMessengerEXT);
      vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_utils_messenger, nullptr);
      m_debug_utils_messenger = VK_NULL_HANDLE;
    }
    if (m_instance != VK_NULL_HANDLE)
    {
      DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyInstance);
      vkDestroyInstance(m_instance, nullptr);
      // Ensure that the parent destructor is a no-op.
      m_instance = VK_NULL_HANDLE;
    }
  }

  void debug_instance_impl::submit_debug_message(const bitmask types, const bitmask severity,
                                                 const std::string& message) const {
    auto callback_data = VkDebugUtilsMessengerCallbackDataEXT{ };
    callback_data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
    callback_data.pMessage = message.data();
    DECLARE_INSTANCE_PFN(*m_idt, vkSubmitDebugUtilsMessageEXT);
    vkSubmitDebugUtilsMessageEXT(m_instance, static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(severity),
                                 static_cast<VkDebugUtilsMessageTypeFlagsEXT>(types),
                                 &callback_data);
  }

}
