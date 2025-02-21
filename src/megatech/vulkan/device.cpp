/**
 * @file device.cpp
 * @brief Vulkan Devices
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#include "megatech/vulkan/device.hpp"

#include <utility>

#include <megatech/assertions.hpp>

#include "megatech/vulkan/physical_devices.hpp"

#include "megatech/vulkan/internal/base/device_impl.hpp"

namespace megatech::vulkan {

  device::device(const physical_device_description& parent) :
  m_impl{ new implementation_type{ parent.share_implementation() } } {
    MEGATECH_POSTCONDITION(m_impl != nullptr);
  }

  device::implementation_type& device::implementation() {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  const device::implementation_type& device::implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return *m_impl;
  }

  std::shared_ptr<const device::implementation_type> device::share_implementation() const {
    MEGATECH_PRECONDITION(m_impl != nullptr);
    return m_impl;
  }

}
