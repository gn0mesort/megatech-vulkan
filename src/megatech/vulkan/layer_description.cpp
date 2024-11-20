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

  bool layer_description::operator==(const layer_description& rhs) const {
    return m_name == rhs.m_name;
  }

  bool layer_description::operator==(const std::string& name) const {
    return m_name == name;
  }

}

namespace std {

  using megatech::vulkan::layer_description;

  std::size_t hash<layer_description>::operator()(const layer_description& layer) const noexcept {
    return std::hash<std::string>{ }(layer.name());
  }

  std::size_t hash<layer_description>::operator()(const std::string& name) const noexcept {
    return std::hash<std::string>{ }(name);
  }

  bool equal_to<layer_description>::operator()(const layer_description& lhs, const layer_description& rhs) const noexcept {
    return lhs == rhs;
  }

  bool equal_to<layer_description>::operator()(const layer_description& lhs, const std::string& rhs) const noexcept {
    return lhs == rhs;
  }

  bool equal_to<layer_description>::operator()(const std::string& lhs, const layer_description& rhs) const noexcept {
    return lhs == rhs;
  }

}
