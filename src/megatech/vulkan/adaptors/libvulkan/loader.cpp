#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

namespace mv = megatech::vulkan;

namespace megatech::vulkan::adaptors::libvulkan {

  loader::loader() :
  mv::loader{ std::shared_ptr<implementation_type>{ new implementation_type{ vkGetInstanceProcAddr } } } { }

}
