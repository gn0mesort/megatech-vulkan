/**
 * @file loader.cpp
 * @brief Standard Vulkan Loaders
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_impl.hpp"

namespace mv = megatech::vulkan;
namespace mvalib = megatech::vulkan::adaptors::libvulkan::internal::base;

namespace megatech::vulkan::adaptors::libvulkan {

  loader::loader() :
  mv::loader{ std::shared_ptr<implementation_type>{ new mvalib::loader_impl{ } } } { }

}
