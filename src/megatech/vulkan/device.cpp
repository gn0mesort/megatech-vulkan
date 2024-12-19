#include "megatech/vulkan/device.hpp"

#include <utility>

#include "megatech/vulkan/internal/base/device_impl.hpp"

namespace megatech::vulkan::internal::base {

  void device_impl_dtor::operator()(device_impl* p) const noexcept {
    delete p;
  }

}

namespace megatech::vulkan {

  device::device(implementation_type*&& impl) :
  m_impl{ std::exchange(impl, nullptr), internal::base::device_impl_dtor{ } } { }

  device::implementation_type& device::implementation() {
    return *m_impl;
  }

  const device::implementation_type& device::implementation() const {
    return *m_impl;
  }

  std::shared_ptr<const device::implementation_type> device::share_implementation() const {
    return m_impl;
  }

}
