#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_LOADER_HPP

#include <megatech/vulkan/loader.hpp>

namespace megatech::vulkan::adaptors::libvulkan {

  class loader final : public megatech::vulkan::loader {
  public:
    loader();
    loader(const loader& other) = delete;
    loader(loader&& other) = delete;

    ~loader() noexcept = default;

    loader& operator=(const loader& rhs) = delete;
    loader& operator=(loader&& rhs) = delete;
  };

}

#endif
