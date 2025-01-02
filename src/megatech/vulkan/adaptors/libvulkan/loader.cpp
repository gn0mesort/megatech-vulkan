#define MEGATECH_VULKAN_INCLUDE_VULKAN_H
#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include <utility>

#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_function_builder.hpp"

namespace megatech::vulkan::adaptors::libvulkan {

  loader::loader() :
  megatech::vulkan::loader{
    new megatech::vulkan::internal::base::loader_impl{ internal::base::loader_function_builder{ } }
  } { }

}
