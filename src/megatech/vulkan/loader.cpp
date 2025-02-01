#include "megatech/vulkan/loader.hpp"

#include <megatech/assertions.hpp>

#include <utility>

#include "megatech/vulkan/internal/base/loader_impl.hpp"

namespace megatech::vulkan {

  loader::loader(const std::shared_ptr<implementation_type>& impl) :
  m_impl{ impl } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  const loader::implementation_type& loader::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  loader::implementation_type& loader::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const loader::implementation_type> loader::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

  const std::unordered_set<layer_description>& loader::available_layers() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl->available_layers();
  }

}
