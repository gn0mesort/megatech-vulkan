#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan::internal::base {

  const dispatch::instance::table& instance_impl::dispatch_table() const {
    return *m_idt;
  }

  VkInstance instance_impl::handle() const {
    return m_instance;
  }

}
