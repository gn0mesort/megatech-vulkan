#include "megatech/vulkan/adaptors/libvulkan/internal/base/loader_function_builder.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  PFN_vkGetInstanceProcAddr loader_function_builder::build() const {
    return vkGetInstanceProcAddr;
  }

}
