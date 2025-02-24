#include "megatech/vulkan/window_system.hpp"

#include <megatech/assertions.hpp>

namespace megatech::vulkan {

  window_system::window_system(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  const window_system::implementation_type& window_system::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  window_system::implementation_type& window_system::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const window_system::implementation_type> window_system::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

}
