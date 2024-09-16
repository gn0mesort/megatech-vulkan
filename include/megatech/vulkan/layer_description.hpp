#ifndef MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP
#define MEGATECH_VULKAN_LAYER_DESCRIPTION_HPP

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
    version m_implementation_version{ 0, 1, 0, 0 };
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

    const version& specification_version() const;
    const version& implementation_version() const;
    const std::string& name() const;
    const std::string& description() const;
  };

}

#endif
