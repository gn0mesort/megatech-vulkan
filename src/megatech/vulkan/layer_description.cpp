#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/layer_description.hpp"

namespace megatech::vulkan {

  layer_description::layer_description(const VkLayerProperties& properties, const internal::tag&) :
  m_specification_version{ properties.specVersion }, m_implementation_version{ properties.implementationVersion },
  m_name{ properties.layerName }, m_description{ properties.description } { }

  const version& layer_description::specification_version() const {
    return m_specification_version;
  }

  std::uint32_t layer_description::implementation_version() const {
    return m_implementation_version;
  }

  const std::string& layer_description::name() const {
    return m_name;
  }

  const std::string& layer_description::description() const {
    return m_description;
  }

}
