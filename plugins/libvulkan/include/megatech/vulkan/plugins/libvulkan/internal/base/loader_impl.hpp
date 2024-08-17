#ifndef MEGATECH_VULKAN_PLUGINS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_PLUGINS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP

#include <megatech/vulkan/internal/base/loader_impl.hpp>

namespace megatech::vulkan::plugins::libvulkan::internal::base {

  class loader_impl final : public megatech::vulkan::internal::base::loader_impl {
  public:
    loader_impl();
    loader_impl(const loader_impl& other);
    loader_impl(loader_impl&& other) = delete;

    ~loader_impl() noexcept = default;

    loader_impl& operator=(const loader_impl& rhs);
    loader_impl& operator=(loader_impl&& rhs) = delete;
  };

}

#endif
