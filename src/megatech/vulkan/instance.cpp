#include "megatech/vulkan/instance.hpp"

#include <utility>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan {

  instance::instance(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  const instance::implementation_type& instance::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  instance::implementation_type& instance::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const instance::implementation_type> instance::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

  debug_instance::debug_instance(const std::shared_ptr<extended_implementation_type>& impl) : instance{ impl } { }

  void debug_instance::submit_debug_message(const bitmask types, const bitmask severity,
                                            const std::string& message) const {
    MEGATECH_PRECONDITION((types & ~debug_messenger_description::default_message_types) == bitmask{ 0 });
    MEGATECH_PRECONDITION((types & debug_messenger_description::default_message_types) != bitmask{ 0 });
    MEGATECH_PRECONDITION(std::has_single_bit(static_cast<std::uint64_t>(severity)));
    MEGATECH_POSTCONDITION((severity & ~debug_messenger_description::default_message_severities) == bitmask{ 0 });
    MEGATECH_PRECONDITION((severity & debug_messenger_description::default_message_severities) != bitmask{ 0 });
    static_cast<const extended_implementation_type&>(implementation()).submit_debug_message(types, severity, message);
  }

}
