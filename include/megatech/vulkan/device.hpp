#ifndef MEGATECH_VULKAN_DEVICE_HPP
#define MEGATECH_VULKAN_DEVICE_HPP

#include <memory>

#include "defs.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class device_impl;

}

namespace megatech::vulkan {

  class device {
  public:
    using implementation_type = internal::base::device_impl;
  protected:
    explicit device(const std::shared_ptr<implementation_type>& impl);

    std::shared_ptr<implementation_type> m_impl{ };
  public:
    device() = delete;
    device(const device& other) = delete;
    device(device&& other) = delete;

    virtual ~device() noexcept = default;

    device& operator=(const device& other) = delete;
    device& operator=(device&& other) = delete;

    implementation_type& implementation();
    const implementation_type& implementation() const;
    std::shared_ptr<const implementation_type> share_implementation() const;
  };

  static_assert(concepts::opaque_object<device>);
  static_assert(concepts::readonly_sharable_opaque_object<device>);

}

#endif
