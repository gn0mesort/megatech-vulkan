#include "megatech/vulkan/internal/base/instance_impl.hpp"

#include <utility>

#include "config.hpp"

#include "megatech/vulkan/internal/base/physical_device_validator.hpp"
#include "megatech/vulkan/internal/base/loader_impl.hpp"

#define DECLARE_GLOBAL_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd)
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

  instance_impl::instance_impl(const std::shared_ptr<const parent_type>& parent,
                               std::unique_ptr<const physical_device_validator>&& validator) :
  m_validator{ std::move(validator) },
  m_parent{ parent } { }

  instance_impl::instance_impl(const std::shared_ptr<const loader_impl>& parent,
                               std::unique_ptr<const physical_device_validator>&& validator,
                               const VkInstanceCreateInfo& instance_info) :
  instance_impl{ parent, std::move(validator) } {
    DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    for (auto i = std::uint32_t{ 0 }; i < instance_info.enabledLayerCount; ++i)
    {
      m_enabled_layers.insert(instance_info.ppEnabledLayerNames[i]);
    }
    for (auto i = std::uint32_t{ 0 }; i < instance_info.enabledExtensionCount; ++i)
    {
      m_enabled_extensions.insert(instance_info.ppEnabledExtensionNames[i]);
    }
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

  const physical_device_validator& instance_impl::validator() const {
    return *m_validator;
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
                                           std::unique_ptr<const physical_device_validator>&& validator) :
  instance_impl{ parent, std::move(validator) } { }

  debug_instance_impl::debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                                           std::unique_ptr<const physical_device_validator>&& validator,
                                           const VkInstanceCreateInfo& instance_info) :
  instance_impl{ parent, std::move(validator), instance_info } { }

  debug_instance_impl::debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                                           std::unique_ptr<const physical_device_validator>&& validator,
                                           const debug_messenger_description& messenger_description,
                                           VkInstanceCreateInfo instance_info) :
  instance_impl{ parent, std::move(validator) },
  m_message_sink{ messenger_description.sink() } {
    auto debug_utils_messenger_info = VkDebugUtilsMessengerCreateInfoEXT{ };
    debug_utils_messenger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_utils_messenger_info.messageSeverity =
      static_cast<VkDebugUtilsMessageSeverityFlagsEXT>(messenger_description.accepted_message_severities());
    debug_utils_messenger_info.messageType =
      static_cast<VkDebugUtilsMessageTypeFlagsEXT>(messenger_description.accepted_message_types());
    debug_utils_messenger_info.pfnUserCallback = vkDebugUtilsMessengerCallbackEXT;
    debug_utils_messenger_info.pUserData = &m_message_sink;
    if (instance_info.pNext)
    {
      debug_utils_messenger_info.pNext = instance_info.pNext;
    }
    instance_info.pNext = &debug_utils_messenger_info;
    DECLARE_GLOBAL_PFN(m_parent->dispatch_table(), vkCreateInstance);
    VK_CHECK(vkCreateInstance(&instance_info, nullptr, &m_instance));
    for (auto i = std::uint32_t{ 0 }; i < instance_info.enabledLayerCount; ++i)
    {
      m_enabled_layers.insert(instance_info.ppEnabledLayerNames[i]);
    }
    for (auto i = std::uint32_t{ 0 }; i < instance_info.enabledExtensionCount; ++i)
    {
      m_enabled_extensions.insert(instance_info.ppEnabledExtensionNames[i]);
    }
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
