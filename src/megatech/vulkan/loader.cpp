#define VK_NO_PROTOTYPES (1)
#define MEGATECH_VULKAN_INCLUDE_VULKAN_H (1)
#include "megatech/vulkan/loader.hpp"

#include <utility>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "megatech/vulkan/layer_description.hpp"

#include "megatech/vulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan::internal::base {

  void loader_impl_dtor::operator()(loader_impl* p) const noexcept {
    delete p;
  }

}

namespace megatech::vulkan {

  const void* loader::resolve(const std::uint_least64_t hash) const {
    return m_impl->dispatch_table()(hash);
  }

  loader::loader(internal::base::loader_impl*&& impl) :
  m_impl{ std::exchange(impl, nullptr), internal::base::loader_impl_dtor{ } } { }

  const loader::implementation_type& loader::implementation() const {
    return *m_impl;
  }

  loader::implementation_type& loader::implementation() {
    return *m_impl;
  }

  std::shared_ptr<const loader::implementation_type> loader::share_implementation() const {
    return m_impl;
  }

  const dispatch::global::table& loader::dispatch_table() const {
    return m_impl->dispatch_table();
  }

  const std::vector<layer_description>& loader::available_layers() const {
    return m_impl->available_layers();
  }


}
