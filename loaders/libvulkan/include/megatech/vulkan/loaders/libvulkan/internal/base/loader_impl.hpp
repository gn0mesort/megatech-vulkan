/// @cond INTERNAL
/**
 * @file loader_impl.hpp
 * @brief Loader Implementation for the Standard Vulkan Loader
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_LOADERS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_LOADERS_LIBVULKAN_INTERNAL_BASE_LOADER_IMPL_HPP

#include <megatech/vulkan/internal/base/loader_impl.hpp>

namespace megatech::vulkan::loaders::libvulkan::internal::base {

  /**
   * @brief An implementation of a loader that leverages libvulkan.1 (a/k/a libvulkan-1).
   */
  class loader_impl final : public megatech::vulkan::internal::base::loader_impl {
  public:
    /**
     * @brief Construct a loader_impl.
     */
    loader_impl();

    /// @cond
    loader_impl(const loader_impl& other) = delete;
    loader_impl(loader_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader_impl.
     */
    ~loader_impl() noexcept = default;

    /// @cond
    loader_impl& operator=(const loader_impl& rhs) = delete;
    loader_impl& operator=(loader_impl&& rhs) = delete;
    /// @endcond
  };

}

#endif
/// @endcond
