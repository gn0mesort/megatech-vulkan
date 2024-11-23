#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP

#include <string>
#include <unordered_set>

#include <megatech/vulkan/instance.hpp>

namespace megatech::vulkan {

  class version;

}

namespace megatech::vulkan::adaptors::libvulkan {

  class loader;

  class instance : public megatech::vulkan::instance {
  public:
    instance() = delete;
    instance(const loader& parent_loader, const std::string& application_name, const version& application_version);
    instance(const loader& parent_loader, const std::string& application_name, const version& application_version,
             const std::unordered_set<std::string>& requested_layers);
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;

    ~instance() noexcept = default;

    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;
  };

}

#endif
