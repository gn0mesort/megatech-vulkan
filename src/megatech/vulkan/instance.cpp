#include "megatech/vulkan/instance.hpp"

#include <utility>

#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan::internal::base {

  void instance_impl_dtor::operator()(instance_impl* p) const noexcept {
    delete p;
  }

}

namespace megatech::vulkan {

  instance::instance(internal::base::instance_impl*&& impl) :
  m_impl{ std::exchange(impl, nullptr), internal::base::instance_impl_dtor{ } } { }

  const instance::implementation_type& instance::implementation() const {
    return *m_impl;
  }

  instance::implementation_type& instance::implementation() {
    return *m_impl;
  }

  std::shared_ptr<const instance::implementation_type> instance::share_implementation() const {
    return m_impl;
  }

}
