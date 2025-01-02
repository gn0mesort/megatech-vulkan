#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP

#include <megatech/vulkan/internal/base/loader_impl.hpp>

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

  class loader_impl final : public megatech::vulkan::internal::base::loader_impl {
  public:
    loader_impl();
    explicit loader_impl(const loader_impl& other);
    loader_impl(loader_impl&& other) = delete;

    ~loader_impl() noexcept = default;

    loader_impl& operator=(const loader_impl& rhs);
    loader_impl& operator=(loader_impl&& rhs) = delete;
  };

}

#endif
