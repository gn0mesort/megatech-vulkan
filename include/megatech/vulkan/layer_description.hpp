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

// It's weird, but one of the few times it's acceptable to open the std namespace is to specialize std::hash.
namespace std {

  template <>
  class hash<megatech::vulkan::layer_description> final {
  public:
    std::size_t operator()(const megatech::vulkan::layer_description& layer) const noexcept {
      return std::hash<std::string>{}(layer.name());
    }
    // Weird but required for find() based on a std::string only. This is C++20 feature.
    std::size_t operator()(const std::string& name) const noexcept {
      return std::hash<std::string>{}(name);
    }
  };

}

#endif
