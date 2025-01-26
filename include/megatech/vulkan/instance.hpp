#ifndef MEGATECH_VULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_INSTANCE_HPP

#include <memory>
#include <vector>

#include "bitmask.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class debug_instance_impl;

}

namespace megatech::vulkan {

  class physical_device_description;

  /**
   * @brief A Vulkan instance.
   * @details Vulkan instances are, more or less, a single instantiation of the Vulkan API. The represent all of the
   *          functionality that is not specific to any hardware device.
   */
  class instance {
  public:
    using implementation_type = internal::base::instance_impl;
  private:
    std::shared_ptr<implementation_type> m_impl{ };
  protected:
    instance() = default;
    explicit instance(const std::shared_ptr<implementation_type>& impl);
  public:
    /// @cond
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy an instance.
     */
    virtual ~instance() = default;

    /// @cond
    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;
    /// @endcond

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
  };

  static_assert(concepts::opaque_object<instance>);
  static_assert(concepts::readonly_sharable_opaque_object<instance>);

  /**
   * @brief A Vulkan instance with debugging instrumentation.
   * @details Debug instances extend the base instance functionality with the VK_EXT_debug_utils extension.
   *          This allows them to generate diagnostic messages from VK_LAYER_KHRONOS_validation, and to provide
   *          extra information to specialized debuggers like RenderDoc.
   */
  class debug_instance : public instance {
  public:
    using extended_implementation_type = internal::base::debug_instance_impl;
  protected:
    explicit debug_instance(const std::shared_ptr<extended_implementation_type>& impl);
  public:
    /// @cond
    debug_instance() = delete;
    debug_instance(const debug_instance& other) = delete;
    debug_instance(debug_instance&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a debug_instance.
     */
    virtual ~debug_instance() = default;

    /// @cond
    debug_instance& operator=(const debug_instance& rhs) = delete;
    debug_instance& operator=(debug_instance&& rhs) = delete;
    /// @endcond

    /**
     * @brief Send a custom debug message through the Vulkan debug API.
     * @param types A set of message types that the custom message belongs to.
     * @param severity The severity of the custom message. This must be exactly one bit (i.e., a power of two).
     * @param message The content of the message.
     */
    void submit_debug_message(const bitmask types, const bitmask severity, const std::string& message) const;
  };

  static_assert(concepts::opaque_object<debug_instance>);
  static_assert(concepts::readonly_sharable_opaque_object<debug_instance>);

}

#endif
