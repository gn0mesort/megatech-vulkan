#ifndef MEGATECH_VULKAN_PHYSICAL_DEVICES_HPP
#define MEGATECH_VULKAN_PHYSICAL_DEVICES_HPP

#include <memory>
#include <vector>

#include "defs.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal {

  struct tag;

}

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class physical_device_description_impl;

}

namespace megatech::vulkan {

namespace async_transfer_support {

  constexpr int none{ 0 };
  constexpr int combined{ 1 };
  constexpr int dedicated{ 2 };

}

  class instance;
  class surface;

  class physical_device_description final {
  public:
    using implementation_type = internal::base::physical_device_description_impl;
  private:
    std::shared_ptr<implementation_type> m_impl{ };
  public:
    physical_device_description(const std::shared_ptr<implementation_type>& impl);
    physical_device_description(const physical_device_description& other) = default;
    physical_device_description(physical_device_description&& other) = delete;

    ~physical_device_description() noexcept = default;

    physical_device_description& operator=(const physical_device_description& rhs) = default;
    physical_device_description& operator=(physical_device_description&& rhs) = delete;
    bool operator==(const physical_device_description& rhs) const noexcept;

    const implementation_type& implementation() const;
    implementation_type& implementation();
    std::shared_ptr<const implementation_type> share_implementation() const;

    bool supports_rendering() const;
    bool supports_presentation(const surface& srfc) const;
    bool supports_async_execution() const;
    int supports_async_transfer() const;
  };

  static_assert(concepts::opaque_object<physical_device_description>);
  static_assert(concepts::readonly_sharable_opaque_object<physical_device_description>);

  class physical_device_list final {
  private:
    std::vector<physical_device_description> m_physical_devices;

    physical_device_list(std::vector<physical_device_description>&& filtered_list);

    bool is_valid(const physical_device_description& physical_device) const;
  public:
    using value_type = physical_device_description;
    using size_type = std::vector<physical_device_description>::size_type;
    using difference_type = std::vector<physical_device_description>::difference_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using const_iterator = std::vector<physical_device_description>::const_iterator;
    using const_reverse_iterator = std::vector<physical_device_description>::const_reverse_iterator;

    physical_device_list() = delete;
    explicit physical_device_list(const instance& inst);
    physical_device_list(const physical_device_list& other) = default;
    physical_device_list(physical_device_list&& other) = delete;

    ~physical_device_list() noexcept = default;

    physical_device_list& operator=(const physical_device_list& rhs) = default;
    physical_device_list& operator=(physical_device_list&& rhs) = delete;
    const_reference operator[](const size_type index) const;
    bool operator==(const physical_device_list& rhs) const noexcept;

    const_reference at(const size_type index) const;
    const_reference front() const;
    const_reference back() const;
    const_pointer data() const;
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    bool empty() const;
    size_type size() const;
  };

}

#endif
