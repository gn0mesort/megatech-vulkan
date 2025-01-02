#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LAYER_DESCRIPTION_PROXY_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LAYER_DESCRIPTION_PROXY_HPP

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  struct layer_description_proxy final {
    VkLayerProperties properties;
  };

}

#endif
