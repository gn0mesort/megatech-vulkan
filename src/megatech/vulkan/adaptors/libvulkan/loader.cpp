#include "megatech/vulkan/adaptors/libvulkan/loader.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"

extern "C" VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance, const char*);

namespace mv = megatech::vulkan;
namespace mvib = megatech::vulkan::internal::base;

namespace megatech::vulkan::adaptors::libvulkan {

  class loader_function_owner final : public mvib::loader_function_owner {
  public:
    loader_function_owner() : mvib::loader_function_owner{ vkGetInstanceProcAddr } { }

    ~loader_function_owner() noexcept = default;
  };

  loader::loader() :
  mv::loader{ []() {
    auto owner = std::shared_ptr<loader_function_owner>{ new loader_function_owner{ } };
    return std::shared_ptr<implementation_type>{ new implementation_type{ owner } };
  }() } { }

}
