/**
 * @file loader_impl.cpp
 * @brief Loader Implementation for the Standard Vulkan Loader
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_impl.hpp"

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

namespace mvib = megatech::vulkan::internal::base;

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  loader_impl::loader_impl() : mvib::loader_impl{ vkGetInstanceProcAddr } { }

}
