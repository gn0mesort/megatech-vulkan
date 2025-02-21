/// @cond INTERNAL
/**
 * @file layer_description_proxy.hpp
 * @brief Proxy Objects for VkLayerProperties
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LAYER_DESCRIPTION_PROXY_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LAYER_DESCRIPTION_PROXY_HPP

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  /**
   * @brief A proxy type to hide megatech::vulkan::layer_description's dependency on Vulkan.
   */
  struct layer_description_proxy final {
    /**
     * @brief The actual underlying VkLayerProperties object used to construct a layer_description.
     */
    VkLayerProperties properties;
  };

}

#endif
/// @endcond
