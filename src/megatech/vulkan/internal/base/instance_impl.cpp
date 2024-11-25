#include "megatech/vulkan/internal/base/instance_impl.hpp"

namespace megatech::vulkan::internal::base {

  bool instance_impl::is_valid(const physical_device_description&) const {
    // The default implementation places no extended criteria on physical device validity. That is to say,
    // all physical devices are valid if they meet the base criteria and the instance implementation hasn't overrode
    // is_valid.
    return true;
  }

  const dispatch::instance::table& instance_impl::dispatch_table() const {
    return *m_idt;
  }

  VkInstance instance_impl::handle() const {
    return m_instance;
  }

}
