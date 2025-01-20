#include "megatech/vulkan/adaptors/libvulkan/instance.hpp"

#include "config.hpp"

#include "megatech/vulkan/loader.hpp"
#include "megatech/vulkan/version.hpp"
#include "megatech/vulkan/layer_description.hpp"
#include "megatech/vulkan/application_description.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/loader_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_validator.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"

#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

namespace mv = megatech::vulkan;
namespace mvib = megatech::vulkan::internal::base;

namespace megatech::vulkan::adaptors::libvulkan {

  instance::instance(const loader& parent, const application_description& description) :
  instance{ parent, description, { } } { }

  instance::instance(const loader& parent, const application_description& description,
                     const std::unordered_set<std::string>& requested_layers) :
  mv::instance{ [](const loader& parent, const application_description& description,
                   const std::unordered_set<std::string>& requested_layers){
    auto application_info = VkApplicationInfo{ };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = description.name().data();
    application_info.applicationVersion = static_cast<std::uint32_t>(description.version());
    application_info.pEngineName = CONFIG_VULKAN_ENGINE_NAME;
    application_info.engineVersion = VK_MAKE_API_VERSION(0, CONFIG_VULKAN_ENGINE_VERSION_MAJOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_MINOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_PATCH);
    application_info.apiVersion = VK_API_VERSION_1_3;
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &application_info;
    auto layers = std::vector<const char*>{ };
    for (auto requested_layer : requested_layers)
    {
      if (parent.available_layers().contains(requested_layer))
      {
        layers.emplace_back(requested_layer.data());
      }
    }
    instance_info.ppEnabledLayerNames = layers.data();
    instance_info.enabledLayerCount = layers.size();
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new implementation_type{ parent.share_implementation(), std::move(validator), instance_info };
    return std::shared_ptr<implementation_type>{ ptr };
  }(parent, description, requested_layers) } { }

  debug_instance::debug_instance(const loader& parent, const application_description& description) :
  debug_instance{ parent, description, { } } { }

  debug_instance::debug_instance(const loader& parent, const application_description& description,
                                 const std::unordered_set<std::string>& requested_layers) :
  mv::debug_instance{ [](const loader& parent, const application_description& description,
                         const std::unordered_set<std::string>& requested_layers) {
    auto application_info = VkApplicationInfo{ };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = description.name().data();
    application_info.applicationVersion = static_cast<std::uint32_t>(description.version());
    application_info.pEngineName = CONFIG_VULKAN_ENGINE_NAME;
    application_info.engineVersion = VK_MAKE_API_VERSION(0, CONFIG_VULKAN_ENGINE_VERSION_MAJOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_MINOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_PATCH);
    application_info.apiVersion = VK_API_VERSION_1_3;
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &application_info;
    auto layers = std::vector<const char*>{ };
    for (const auto& requested_layer : requested_layers)
    {
      if (parent.available_layers().contains(requested_layer))
      {
        layers.emplace_back(requested_layer.data());
      }
    }
    instance_info.ppEnabledLayerNames = layers.data();
    instance_info.enabledLayerCount = layers.size();
    auto extensions = std::vector<const char*>{ "VK_EXT_debug_utils" };
    instance_info.ppEnabledExtensionNames = extensions.data();
    instance_info.enabledExtensionCount = extensions.size();
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new extended_implementation_type{ parent.share_implementation(), std::move(validator), instance_info };
    return std::shared_ptr<extended_implementation_type>{ ptr };
  }(parent, description, requested_layers) } { }

  debug_instance::debug_instance(const loader& parent, const application_description& description,
                                 const std::unordered_set<std::string>& requested_layers,
                                 const debug_messenger_description& messenger_description) :
  mv::debug_instance{ [](const loader& parent, const application_description& description,
                         const std::unordered_set<std::string>& requested_layers,
                         const debug_messenger_description& messenger_description) {
    auto application_info = VkApplicationInfo{ };
    application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info.pApplicationName = description.name().data();
    application_info.applicationVersion = static_cast<std::uint32_t>(description.version());
    application_info.pEngineName = CONFIG_VULKAN_ENGINE_NAME;
    application_info.engineVersion = VK_MAKE_API_VERSION(0, CONFIG_VULKAN_ENGINE_VERSION_MAJOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_MINOR,
                                                         CONFIG_VULKAN_ENGINE_VERSION_PATCH);
    application_info.apiVersion = VK_API_VERSION_1_3;
    auto instance_info = VkInstanceCreateInfo{ };
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &application_info;
    auto layers = std::vector<const char*>{ };
    for (const auto& requested_layer : requested_layers)
    {
      if (parent.available_layers().contains(requested_layer))
      {
        layers.emplace_back(requested_layer.data());
      }
    }
    instance_info.ppEnabledLayerNames = layers.data();
    instance_info.enabledLayerCount = layers.size();
    auto extensions = std::vector<const char*>{ "VK_EXT_debug_utils" };
    instance_info.ppEnabledExtensionNames = extensions.data();
    instance_info.enabledExtensionCount = extensions.size();
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new extended_implementation_type{ parent.share_implementation(), std::move(validator),
                                                 messenger_description, instance_info };
    return std::shared_ptr<extended_implementation_type>{ ptr };
  }(parent, description, requested_layers, messenger_description) } { }

}
