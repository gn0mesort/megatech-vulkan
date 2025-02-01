#include "megatech/vulkan/internal/base/instance_impl.hpp"

#include <bit>
#include <utility>
#include <algorithm>

#include <megatech/assertions.hpp>

#include "config.hpp"

#include "megatech/vulkan/application_description.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
#define DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd)
#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

namespace {

  VKAPI_ATTR VkBool32 VKAPI_CALL
  vkDebugUtilsMessengerCallbackEXT(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                   VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                   void* pUserData) {
    constexpr auto all_severities = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    constexpr auto all_types = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    MEGATECH_PRECONDITION(std::has_single_bit(static_cast<std::uint64_t>(messageSeverity)));
    MEGATECH_PRECONDITION((messageSeverity & all_severities) != 0);
    MEGATECH_PRECONDITION((messageTypes & all_types) != 0);
    using megatech::vulkan::debug_messenger_description;
    using megatech::vulkan::bitmask;
    if (pUserData)
    {
      const auto& sink = *reinterpret_cast<std::function<debug_messenger_description::message_sink_fn>*>(pUserData);
      sink(static_cast<bitmask>(messageTypes), static_cast<bitmask>(messageSeverity), pCallbackData->pMessage);
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
  m_parent{ parent } {
    if (!parent)
    {
      throw error{ "The parent loader cannot be null." };
    }
    // Because of the move operation, this has to be validated through the member pointer.
    if (!m_physical_device_allocator)
    {
      throw error{ "The physical_device_allocator cannot be null." };
    }
    MEGATECH_POSTCONDITION(m_parent != nullptr);
    MEGATECH_POSTCONDITION(m_parent == parent);
    MEGATECH_POSTCONDITION(m_physical_device_allocator != nullptr);
  }

  instance_impl::instance_impl(const std::shared_ptr<const loader_impl>& parent,
                               std::unique_ptr<const class physical_device_allocator>&& allocator,
                               const application_description& app_description,
                               const instance_description& description) :
  m_physical_device_allocator{ std::move(allocator) },
  m_parent{ parent } {
    if (!parent)
    {
      throw error{ "The parent loader cannot be null." };
    }
    if (!m_physical_device_allocator)
    {
      throw error{ "The physical_device_allocator cannot be null." };
    }
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
    auto instance = VkInstance{ };
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &instance));
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), instance });
    MEGATECH_POSTCONDITION(m_parent != nullptr);
    MEGATECH_POSTCONDITION(m_parent == parent);
    MEGATECH_POSTCONDITION(m_physical_device_allocator != nullptr);
    MEGATECH_POSTCONDITION(m_idt != nullptr);
    MEGATECH_POSTCONDITION(m_idt->instance() == instance);
  }

  instance_impl::~instance_impl() noexcept {
    // This allows changing destruction behavior in a derived class (e.g., debug_instance_impl). A derived class
    // can destroy the dispatch table on its own and then null the pointer to prevent a double free situation.
    if (m_idt)
    {
      DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyInstance);
      vkDestroyInstance(m_idt->instance(), nullptr);
    }
  }

  const class physical_device_allocator& instance_impl::physical_device_allocator() const {
    MEGATECH_PRECONDITION(m_physical_device_allocator != nullptr);
    return *m_physical_device_allocator;
  }

  const dispatch::instance::table& instance_impl::dispatch_table() const {
    MEGATECH_PRECONDITION(m_idt != nullptr);
    return *m_idt;
  }

  instance_impl::handle_type instance_impl::handle() const {
    MEGATECH_PRECONDITION(m_idt != nullptr);
    return m_idt->instance();
  }

  const instance_impl::parent_type& instance_impl::parent() const {
    MEGATECH_PRECONDITION(m_parent != nullptr);
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
    auto instance = VkInstance{ };
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &instance));
    m_idt.reset(new dispatch::instance::table{ m_parent->dispatch_table(), instance });
    DECLARE_INSTANCE_PFN(*m_idt, vkCreateDebugUtilsMessengerEXT);
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_idt->instance(), &debug_utils_messenger_info, nullptr,
                                            &m_debug_utils_messenger));
    MEGATECH_POSTCONDITION(m_idt != nullptr);
    MEGATECH_POSTCONDITION(m_idt->instance() == instance);
    MEGATECH_POSTCONDITION(m_debug_utils_messenger != VK_NULL_HANDLE);
    MEGATECH_POSTCONDITION(m_message_sink != nullptr);
  }

  debug_instance_impl::~debug_instance_impl() noexcept {
    DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyDebugUtilsMessengerEXT);
    vkDestroyDebugUtilsMessengerEXT(m_idt->instance(), m_debug_utils_messenger, nullptr);
    // The instance must be destoryed here to ensure that m_message_sink is valid during destruction.
    DECLARE_INSTANCE_PFN_NO_THROW(*m_idt, vkDestroyInstance);
    vkDestroyInstance(m_idt->instance(), nullptr);
    // Ensure that the parent destructor is a no-op.
    m_idt.reset();
    MEGATECH_POSTCONDITION(m_idt == nullptr);
  }

  void debug_instance_impl::submit_debug_message(const bitmask types, const bitmask severity,
                                                 const std::string& message) const {
    MEGATECH_PRECONDITION((types & ~debug_messenger_description::default_message_types) == bitmask{ 0 });
    MEGATECH_PRECONDITION((types & debug_messenger_description::default_message_types) != bitmask{ 0 });
    MEGATECH_PRECONDITION(std::has_single_bit(static_cast<std::uint64_t>(severity)));
    MEGATECH_POSTCONDITION((severity & ~debug_messenger_description::default_message_severities) == bitmask{ 0 });
    MEGATECH_PRECONDITION((severity & debug_messenger_description::default_message_severities) != bitmask{ 0 });
    auto callback_data = VkDebugUtilsMessengerCallbackDataEXT{ };
    callback_data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
    callback_data.pMessage = message.data();
    DECLARE_INSTANCE_PFN(*m_idt, vkSubmitDebugUtilsMessageEXT);
    vkSubmitDebugUtilsMessageEXT(m_idt->instance(), static_cast<VkDebugUtilsMessageSeverityFlagBitsEXT>(severity),
                                 static_cast<VkDebugUtilsMessageTypeFlagsEXT>(types),
                                 &callback_data);
  }

}
