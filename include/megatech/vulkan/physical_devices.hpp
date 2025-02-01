#ifndef MEGATECH_VULKAN_PHYSICAL_DEVICES_HPP
#define MEGATECH_VULKAN_PHYSICAL_DEVICES_HPP

#include <memory>
#include <vector>

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal {

  struct tag;

}

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class physical_device_description_impl;

}

namespace megatech::vulkan {

/**
 * @brief A namespace containing constants representing support for asynchronous transfer operations.
 */
namespace async_transfer_support {

  /**
   * @brief No asynchronous transfer operations are supported.
   */
  constexpr int none{ 0 };

  /**
   * @brief Asynchronous transfer operations are supported on a queue that services other operations.
   */
  constexpr int combined{ 1 };

  /**
   * @brief Asynchronous transfer operations are supported on a dedicated queue.
   */
  constexpr int dedicated{ 2 };

}

  class instance;
  class surface;

  /**
   * @brief A description of a Vulkan physical device.
   * @details Physical devices are individual Vulkan devices available to the system. They might, in fact, not be
   *          physical at all! Physical devices are usually discrete GPUs or CPU-integrated GPUs. However, they
   *          can also be software rendering drivers or virtualized GPUs.
   */
  class physical_device_description final {
  public:
    using implementation_type = internal::base::physical_device_description_impl;
  private:
    std::shared_ptr<implementation_type> m_impl{ };
  public:
    /**
     * @brief Construct a physical_device_description.
     * @details This constructor is invoked by the API to generate descriptions based on results returned directly by
     *          the associated Vulkan instance. Unless you know what you are doing, you shouldn't invoke this.
     * @param impl A shared pointer to an opaque physical device implementation object.
     */
    physical_device_description(const std::shared_ptr<implementation_type>& impl);

    /**
     * @brief Copy a physical_device_description.
     * @param other The physical_device_description to copy.
     */
    physical_device_description(const physical_device_description& other) = default;

    /// @cond
    physical_device_description(physical_device_description&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a physical_device_description.
     */
    ~physical_device_description() noexcept = default;

    /**
     * @brief Copy-assign a physical_device_description.
     * @param rhs The physical_device_description to copy.
     * @return A reference to the copied-to physical_device_description.
     */
    physical_device_description& operator=(const physical_device_description& rhs) = default;

    /// @cond
    physical_device_description& operator=(physical_device_description&& rhs) = delete;
    /// @endcond

    /**
     * @brief Compare two physical_device_descriptions for equality.
     * @param rhs The physical_device_description to compare to.
     * @return True if both descriptions describe the same physical device. False otherwise.
     */
    bool operator==(const physical_device_description& rhs) const noexcept;

    /**
     * @brief Retrieve an opaque reference to the underlying implementation.
     * @return A reference to the underlying implementation.
     */
    const implementation_type& implementation() const;

    /**
     * @brief Retrieve an opaque reference to the underlying implementation.
     * @return A reference to the underlying implementation.
     */
    implementation_type& implementation();

    /**
     * @brief Retrieve a sharable reference to the underlying implementation.
     * @details share_implementation() can be used to extend the life of the underlying implementation object. This
     *          is used, primarily, to ensure valid ownership in dependent objects.
     * @return A shareable reference to the underlying implmentation.
     */
    std::shared_ptr<const implementation_type> share_implementation() const;

    /**
     * @brief Check whether or not a described physical device supports rendering (i.e., graphics) operations.
     * @return True if rendering is supported. False otherwise.
     */
    bool supports_rendering() const;

    /**
     * @brief Check whether or not a described physical device supports presentation operations.
     * @param srfc A surface object to query for presentation support.
     * @return True if rendering is supported. False otherwise.
     */
    bool supports_presentation(const surface& srfc) const;

    /**
     * @brief Check whether or not a described physical device supports asynchronous execution (i.e., compute)
     *        operations.
     * @return True if asynchronous execution is supported. False otherwise.
     */
    bool supports_async_execution() const;

    /**
     * @brief Check whether or not a described physical device supports asynchronous transfer operations.
     * @return async_transfer_support::dedicated if a dedicated asynchronous transfer queue is available.
     *         async_transfer_support::combined if supports_async_execution() is true and a dedicated asynchronous
     *         transfer queue isn't available. async_transfer_support::none otherwise.
     */
    int supports_async_transfer() const;
  };

  static_assert(concepts::opaque_object<physical_device_description>);
  static_assert(concepts::readonly_sharable_opaque_object<physical_device_description>);

  /**
   * @brief A list of Vulkan physical_device_descriptions.
   * @details physical_device_lists are, essentially, an immutable collection of physical_device_description objects.
   *          The behave similar to STL collections.
   */
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

    /// @cond
    physical_device_list() = delete;
    /// @endcond

    /**
     * @brief Construct a physical_device_list.
     * @param inst The instance object that the listed devices belong to.
     */
    explicit physical_device_list(const instance& inst);

    /**
     * @brief Copy a physical_device_list.
     * @param other The physical_device_list to copy.
     */
    physical_device_list(const physical_device_list& other) = default;

    /// @cond
    physical_device_list(physical_device_list&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a physical_device_list.
     */
    ~physical_device_list() noexcept = default;

    /**
     * @brief Copy-assign a physical_device_list.
     * @param rhs The physical_device_list to copy.
     * @return A reference to the copied-to physical_device_list.
     */
    physical_device_list& operator=(const physical_device_list& rhs) = default;

    /// @cond
    physical_device_list& operator=(physical_device_list&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the physical_device_description at the given index.
     * @param index The index of the desired physical_device_description. No bounds checking is performed.
     * @return A read-only reference to the desired physical_device_description.
     */
    const_reference operator[](const size_type index) const;

    /**
     * @brief Compare two physical_device_lists for equality.
     * @param rhs The physical_device_list to compare to.
     * @return True if both lists contain the same physical_device_descriptions. False otherwise.
     */
    bool operator==(const physical_device_list& rhs) const noexcept;

    /**
     * @brief Retrieve the physical_device_description at the given index.
     * @param index The index of the desired physical_device_description. Bounds checking is performed.
     * @return A read-only reference to the desired physical_device_description.
     */
    const_reference at(const size_type index) const;

    /**
     * @brief Retrieve the first physical_device_description in the collection.
     * @details Invoking this on an empty container is undefined.
     * @return A read-only reference to the first physical_device_description in the collection.
     */
    const_reference front() const;

    /**
     * @brief Retrieve the last physical_device_description in the collection.
     * @details Invoking this on an empty container is undefined.
     * @return A read-only reference to the last physical_device_description in the collection.
     */
    const_reference back() const;

    /**
     * @brief Retrieve a pointer to the collection's underlying storage.
     * @return A pointer to the underlying storage.
     */
    const_pointer data() const;

    /**
     * @brief Retrieve an iterator to the beginning of the collection.
     * @return An iterator to the beginning of the collection.
     */
    const_iterator begin() const;

    /**
     * @brief Retrieve an iterator to the end of the collection.
     * @return An iterator to the end of the collection.
     */
    const_iterator end() const;

    /**
     * @brief Retrieve a read-only iterator to the beginning of the collection.
     * @return An iterator to the beginning of the collection.
     */
    const_iterator cbegin() const;

    /**
     * @brief Retrieve a read-only iterator to the end of the collection.
     * @return An iterator to the end of the collection.
     */
    const_iterator cend() const;

    /**
     * @brief Retrieve a reverse iterator to the end of the collection.
     * @return A reverse iterator to the end of the collection.
     */
    const_reverse_iterator rbegin() const;

    /**
     * @brief Retrieve a reverse iterator to the beginning of the collection.
     * @return A reverse iterator to the beginning of the collection.
     */
    const_reverse_iterator rend() const;

    /**
     * @brief Retrieve a read-only reverse iterator to the end of the collection.
     * @return A reverse iterator to the end of the collection.
     */
    const_reverse_iterator crbegin() const;

    /**
     * @brief Retrieve a read-only reverse iterator to the beginning of the collection.
     * @return A reverse iterator to the beginning of the collection.
     */
    const_reverse_iterator crend() const;

    /**
     * @brief Determine whether or not the collection is empty.
     * @return True if the collection is empty. False otherwise.
     */
    bool empty() const;

    /**
     * @brief Retrieve the size of the collection.
     * @return The size of the collection.
     */
    size_type size() const;
  };

}

#endif
