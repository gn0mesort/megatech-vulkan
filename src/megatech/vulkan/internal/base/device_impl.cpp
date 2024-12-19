#include "megatech/vulkan/internal/base/device_impl.hpp"

namespace megatech::vulkan::internal::base {

  const dispatch::device::table& device_impl::dispatch_table() const {
    return *m_ddt;
  }

  device_impl::handle_type device_impl::handle() const {
    return m_device;
  }

  const device_impl::parent_type& device_impl::parent() const {
    return *m_parent;
  }

  const std::unordered_set<std::string>& device_impl::enabled_extensions() const {
    return m_enabled_extensions;
  }

}
