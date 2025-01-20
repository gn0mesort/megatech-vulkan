#ifndef MEGATECH_VULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_INSTANCE_HPP

#include <memory>
#include <vector>

#include "defs.hpp"
#include "bitmask.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class debug_instance_impl;

}

namespace megatech::vulkan {

  class physical_device_description;

  class instance {
  public:
    using implementation_type = internal::base::instance_impl;
  private:
    std::shared_ptr<implementation_type> m_impl{ };
  protected:
    instance() = default;
    explicit instance(const std::shared_ptr<implementation_type>& impl);
  public:
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;

    virtual ~instance() = default;

    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;
  };

  static_assert(concepts::opaque_object<instance>);
  static_assert(concepts::readonly_sharable_opaque_object<instance>);

  class debug_instance : public instance {
  public:
    using extended_implementation_type = internal::base::debug_instance_impl;
  protected:
    debug_instance() = default;
    explicit debug_instance(const std::shared_ptr<extended_implementation_type>& impl);
  public:
    debug_instance(const debug_instance& other) = delete;
    debug_instance(debug_instance&& other) = delete;

    virtual ~debug_instance() = default;

    debug_instance& operator=(const debug_instance& rhs) = delete;
    debug_instance& operator=(debug_instance&& rhs) = delete;

    void submit_debug_message(const bitmask types, const bitmask severity, const std::string& message) const;
  };

  static_assert(concepts::opaque_object<debug_instance>);
  static_assert(concepts::readonly_sharable_opaque_object<debug_instance>);

}

#endif
