#ifndef MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP
#define MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP

#include <cstddef>

#include <string>

#if defined(MEGATECH_VULKAN_INCLUDE_VULKAN_H) && __has_include(<vulkan/vulkan.h>)
  #include <vulkan/vulkan.h>
#endif

#include "version.hpp"

namespace megatech::vulkan::internal {

  struct tag;

}

namespace megatech::vulkan {

  class layer_description final {
  private:
    version m_specification_version{ 0, 1, 0, 0 };
    std::uint32_t m_implementation_version{ 1 };
    std::string m_name{ };
    std::string m_description{ };
  public:
#ifdef VK_VERSION_1_0
    layer_description(const VkLayerProperties& properties, const internal::tag&);
#endif
    layer_description(const layer_description& other) = default;
    layer_description(layer_description&& other) = default;

    ~layer_description() noexcept = default;

    layer_description& operator=(const layer_description& rhs) = default;
    layer_description& operator=(layer_description&& rhs) = default;
    bool operator==(const layer_description& rhs) const;
    bool operator==(const std::string& name) const;

    const version& specification_version() const;
    std::uint32_t implementation_version() const;
    const std::string& name() const;
    const std::string& description() const;
  };

}

// It's weird, but one of the few times it's acceptable to open the std namespace is to specialize template structs.
namespace std {

  // These templates are provided to enable heterogenous access in std::unordered_set<layer_description>.
  // In essence, you can search using strings in a set of layer_descriptions. This is because Vulkan uses the layer
  // name as a unique ID for the layer.
  //
  // The "is_transparent" symbol must be defined to enable heterogenous access even with the correct method overloads
  // declared.
  template <>
  struct hash<megatech::vulkan::layer_description> final {
    using is_transparent = void;

    std::size_t operator()(const megatech::vulkan::layer_description& layer) const noexcept;
    std::size_t operator()(const std::string& name) const noexcept;
  };

  template <>
  struct equal_to<megatech::vulkan::layer_description> final {
    using is_transparent = void;

    bool operator()(const megatech::vulkan::layer_description& lhs, const megatech::vulkan::layer_description& rhs) const noexcept;
    bool operator()(const megatech::vulkan::layer_description& lhs, const std::string& rhs) const noexcept;
    bool operator()(const std::string& lhs, const megatech::vulkan::layer_description& rhs) const noexcept;
  };

}

#endif
