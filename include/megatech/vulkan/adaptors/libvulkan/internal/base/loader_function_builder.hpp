#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_LOADER_FUNCTION_BUILDER_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_LOADER_FUNCTION_BUILDER_HPP

#include "../../../../internal/base/loader_impl.hpp"

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  class loader_function_builder final : public megatech::vulkan::internal::base::loader_function_builder {
  public:
    loader_function_builder() = default;
    loader_function_builder(const loader_function_builder& other) = default;
    loader_function_builder(loader_function_builder&& other) = default;

    ~loader_function_builder() noexcept = default;

    loader_function_builder& operator=(const loader_function_builder& rhs) = default;
    loader_function_builder& operator=(loader_function_builder&& rhs) = default;

    PFN_vkGetInstanceProcAddr build() const override;
  };

}

#endif
