#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP

#include <megatech/vulkan/loader.hpp>

namespace megatech::vulkan::adaptors::libvulkan {

  /**
   * @brief A loader using libvulkan as its underlying implementation.
   */
  class loader final : public megatech::vulkan::loader {
  public:
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
