#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP

#include <megatech/vulkan/loader.hpp>

namespace megatech::vulkan::adaptors::libvulkan {

  class loader final : public megatech::vulkan::loader {
  public:
    loader();
    explicit loader(const loader& other);
    loader(loader&& other) = delete;

    ~loader() noexcept = default;

    loader& operator=(const loader& rhs);
    loader& operator=(loader&& rhs) = delete;
  };

}

#endif
