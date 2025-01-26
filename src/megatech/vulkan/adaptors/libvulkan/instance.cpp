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

  instance::instance(const loader& parent, const std::unordered_set<std::string>& requested_layers) :
  instance{ parent, { "", { 0, 1, 0, 0 } }, requested_layers } { }

  instance::instance(const loader& parent, const application_description& app_description) :
  instance{ parent, app_description, { } } { }

  instance::instance(const loader& parent, const application_description& app_description,
                     const std::unordered_set<std::string>& requested_layers) :
  mv::instance{ [](const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers){
    auto description = mvib::instance_description{ requested_layers, { }, { } };
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new implementation_type{ parent.share_implementation(), std::move(validator), app_description,
                                        description };
    return std::shared_ptr<implementation_type>{ ptr };
  }(parent, app_description, requested_layers) } { }

  debug_instance::debug_instance(const loader& parent, const std::unordered_set<std::string>& requested_layers) :
  debug_instance{ parent, { "", { 0, 1, 0, 0 } }, requested_layers } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description) :
  debug_instance{ parent, app_description, { } } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description,
                                 const std::unordered_set<std::string>& requested_layers) :
  mv::debug_instance{ [](const loader& parent, const application_description& app_description,
                         const std::unordered_set<std::string>& requested_layers) {
    auto description = mvib::instance_description{ requested_layers, { }, { "VK_EXT_debug_utils" } };
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new extended_implementation_type{ parent.share_implementation(), std::move(validator), app_description,
                                                 description };
    return std::shared_ptr<extended_implementation_type>{ ptr };
  }(parent, app_description, requested_layers) } { }

  debug_instance::debug_instance(const loader& parent, const application_description& app_description,
                                 const std::unordered_set<std::string>& requested_layers,
                                 const debug_messenger_description& messenger_description) :
  mv::debug_instance{ [](const loader& parent, const application_description& app_description,
                         const std::unordered_set<std::string>& requested_layers,
                         const debug_messenger_description& messenger_description) {
    auto description = mvib::instance_description{ requested_layers, { }, { "VK_EXT_debug_utils" } };
    auto validator = std::unique_ptr<mvib::physical_device_validator>{ new mvib::physical_device_validator{ } };
    auto ptr = new extended_implementation_type{ parent.share_implementation(), std::move(validator), app_description,
                                                 messenger_description, description };
    return std::shared_ptr<extended_implementation_type>{ ptr };
  }(parent, app_description, requested_layers, messenger_description) } { }

}
