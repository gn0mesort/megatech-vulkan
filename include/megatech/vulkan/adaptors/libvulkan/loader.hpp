/**
 * @file loader.hpp
 * @brief Standard Vulkan Loaders
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP

#include <megatech/vulkan/loader.hpp>

namespace megatech::vulkan::adaptors::libvulkan::internal::base {

  class loader_impl;

}

namespace megatech::vulkan::adaptors::libvulkan {

  /**
   * @brief A loader using libvulkan as its underlying implementation.
   */
  class loader final : public megatech::vulkan::loader {
  public:
    /**
     * @brief The derived implementation type of the adapted loader.
     */
    using implementation_type = internal::base::loader_impl;

    /**
     * @brief Construct a loader.
     */
    loader();

    /// @cond
    loader(const loader& other) = delete;
    loader(loader&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader.
     */
    ~loader() noexcept = default;

    /// @cond
    loader& operator=(const loader& rhs) = delete;
    loader& operator=(loader&& rhs) = delete;
    /// @endcond
  };

}

#endif
