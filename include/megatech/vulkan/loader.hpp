#ifndef MEGATECH_VULKAN_LOADER_HPP
#define MEGATECH_VULKAN_LOADER_HPP

#include <memory>
#include <unordered_set>

#include "defs.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class loader_impl;
  struct loader_impl_dtor final {
    void operator()(loader_impl* p) const noexcept;
  };

}

namespace megatech::vulkan {

  class layer_description;

  class loader {
  public:
    using implementation_type = internal::base::loader_impl;
  protected:
    explicit loader(implementation_type*&& impl);

    std::shared_ptr<implementation_type> m_impl;
  public:
    loader() = delete;
    loader(const loader& other) = delete;
    loader(loader&& other) = delete;

    virtual ~loader() noexcept = default;

    loader& operator=(const loader& rhs) = delete;
    loader& operator=(loader&& rhs) = delete;

    const std::unordered_set<layer_description>& available_layers() const;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;
  };


  static_assert(concepts::opaque_object<loader>);
  static_assert(concepts::readonly_sharable_opaque_object<loader>);

}

#endif
