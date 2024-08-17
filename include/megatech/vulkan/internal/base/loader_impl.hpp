#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#include <memory>

#include "../../loader.hpp"

namespace megatech::vulkan::internal::base {

  class loader_impl {
  protected:
    loader_impl() = default;

    std::unique_ptr<dispatch::global::table> m_gdt{ };
  public:
    loader_impl(const loader_impl& other) = delete;
    loader_impl(loader_impl&& other) = delete;

    virtual ~loader_impl() noexcept = default;

    loader_impl& operator=(const loader_impl& rhs) = delete;
    loader_impl& operator=(loader_impl&& rhs) = delete;

    const dispatch::global::table& dispatch_table() const;
  };

}

#endif
