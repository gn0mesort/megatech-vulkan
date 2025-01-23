#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP

#include <unordered_set>

#include <megatech/vulkan/instance.hpp>

namespace megatech::vulkan {

  class version;
  class application_description;
  class debug_messenger_description;

}

namespace megatech::vulkan::adaptors::libvulkan {

  class loader;

  class instance final : public megatech::vulkan::instance {
  public:
    instance() = delete;
    instance(const loader& parent, const application_description& app_description);
    instance(const loader& parent, const application_description& app_description,
             const std::unordered_set<std::string>& requested_layers);
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;

    ~instance() noexcept = default;

    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;
  };

  class debug_instance final : public megatech::vulkan::debug_instance {
  public:
    debug_instance() = delete;
    debug_instance(const loader& parent, const application_description& app_description);
    debug_instance(const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers);
    debug_instance(const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers,
                   const debug_messenger_description& messenger_description);
    debug_instance(const debug_instance& other) = delete;
    debug_instance(debug_instance&& other) = delete;

    ~debug_instance() noexcept = default;

    debug_instance& operator=(const debug_instance& rhs) = delete;
    debug_instance& operator=(debug_instance&& rhs) = delete;
  };

}

#endif
