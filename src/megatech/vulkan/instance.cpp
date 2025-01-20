#include "megatech/vulkan/instance.hpp"

#include <utility>

#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan {

  instance::instance(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } { }

  const instance::implementation_type& instance::implementation() const {
    return *m_impl;
  }

  instance::implementation_type& instance::implementation() {
    return *m_impl;
  }

  std::shared_ptr<const instance::implementation_type> instance::share_implementation() const {
    return m_impl;
  }

  debug_instance::debug_instance(const std::shared_ptr<extended_implementation_type>& impl) : instance{ impl } { }

  void debug_instance::submit_debug_message(const bitmask types, const bitmask severity,
                                            const std::string& message) const {
    static_cast<const extended_implementation_type&>(implementation()).submit_debug_message(types, severity, message);
  }

}
