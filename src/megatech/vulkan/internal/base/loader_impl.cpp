#include "megatech/vulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan::internal::base {

  const dispatch::global::table& loader_impl::dispatch_table() const {
    return *m_gdt;
  }

}
