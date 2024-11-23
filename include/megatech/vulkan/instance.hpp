#ifndef MEGATECH_VULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_INSTANCE_HPP

#include <memory>
#include <vector>

#include "defs.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  struct instance_impl_dtor final {
    void operator()(instance_impl* p) const noexcept;
  };

}

namespace megatech::vulkan {

  class physical_device_description;

  class instance {
  public:
    using implementation_type = internal::base::instance_impl;
  protected:
    instance(implementation_type*&& impl);

    std::shared_ptr<implementation_type> m_impl{ };
  public:
    instance() = delete;
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;

    virtual ~instance() = default;

    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;
  };

  MEGATECH_VULKAN_ENFORCE_CONCEPT(concepts::opaque_object<instance>);
  MEGATECH_VULKAN_ENFORCE_CONCEPT(concepts::readonly_sharable_opaque_object<instance>);

}

#endif
