#include "megatech/vulkan/loader.hpp"

#include <utility>

#include "megatech/vulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan::internal::base {

  void loader_impl_dtor::operator()(loader_impl* p) const noexcept {
    delete p;
  }

}

namespace megatech::vulkan {

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

  const std::unordered_set<layer_description>& loader::available_layers() const {
    return m_impl->available_layers();
  }


}
