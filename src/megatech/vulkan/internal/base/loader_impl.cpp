#include "megatech/vulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan::internal::base {

  const dispatch::global::table& loader_impl::dispatch_table() const {
    return *m_gdt;
  }

  const std::vector<layer_description>& loader_impl::available_layers() const {
    return m_available_layers;
  }

}
