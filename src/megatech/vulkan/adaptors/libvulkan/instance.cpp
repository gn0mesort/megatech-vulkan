#include "megatech/vulkan/adaptors/libvulkan/instance.hpp"

#include "megatech/vulkan/adaptors/libvulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan::adaptors::libvulkan {

  instance::instance(const loader& parent_loader, const std::string& application_name,
                     const version& application_version) :
  instance{ parent_loader, application_name, application_version, { } } { }

  instance::instance(const loader& parent_loader, const std::string& application_name,
                     const version& application_version, const std::unordered_set<std::string>& requested_layers) :
  megatech::vulkan::instance{
    std::move(new internal::base::instance_impl{ parent_loader, application_name, application_version,
                                                 requested_layers })
  } { }

}
