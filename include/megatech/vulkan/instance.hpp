/**
 * @file instance.hpp
 * @brief Vulkan Instances
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_INSTANCE_HPP

#include <memory>
#include <unordered_set>

#include "bitmask.hpp"

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class debug_instance_impl;

}

namespace megatech::vulkan {

  class loader;
  class application_description;
  class debug_messenger_description;
  class window_system;

  /**
   * @brief A Vulkan instance.
   * @details Vulkan instances are, more or less, a single instantiation of the Vulkan API. The represent all of the
   *          functionality that is not specific to any hardware device.
   */
  class instance {
  public:
    /**
     * @brief The internal implementation type of the instance.
     */
    using implementation_type = internal::base::instance_impl;
  private:
    std::shared_ptr<implementation_type> m_impl{ };
  protected:
    /**
     * @brief Construct an instance.
     * @details This is an inheriting constructor.
     * @param impl A shared_ptr pointer to the implementation. This must not be null.
     */
    explicit instance(const std::shared_ptr<implementation_type>& impl);
  public:
    /// @cond
    instance() = delete;
    /// @endcond

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application constructing the instance.
     */
    instance(const loader& parent, const application_description& app_description);

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param wsi A window_system to integrate into the instance.
     * @param app_description A description of the application constructing the instance.
     */
    instance(const loader& parent, const window_system& wsi, const application_description& app_description);

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application constructing the instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    instance(const loader& parent, const application_description& app_description,
             const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param wsi A window_system to integrate into the instance.
     * @param app_description A description of the application constructing the instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    instance(const loader& parent, const window_system& wsi, const application_description& app_description,
             const std::unordered_set<std::string>& requested_layers);

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
  class debug_instance final : public instance {
  public:
    /**
     * @brief The extended internal implementation type of the debug_instance.
     */
    using extended_implementation_type = internal::base::debug_instance_impl;
  protected:
    /**
     * @brief Construct a debug_instance.
     * @details This is an inheriting constructor.
     * @param impl A shared_ptr pointer to the implementation. This must not be null.
     */
    explicit debug_instance(const std::shared_ptr<extended_implementation_type>& impl);
  public:
    /// @cond
    debug_instance() = delete;
    /// @endcond

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application constructing the debug_instance.
     */
    debug_instance(const loader& parent, const application_description& app_description);

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param wsi A window_system to integrate into the debug_instance.
     * @param app_description A description of the application constructing the debug_instance.
     */
    debug_instance(const loader& parent, const window_system& wsi, const application_description& app_description);

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application constructing the debug_instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance(const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param wsi A window_system to integrate into the debug_instance.
     * @param app_description A description of the application constructing the debug_instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance(const loader& parent, const window_system& wsi, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application constructing the debug_instance.
     * @param messenger_description A description of a debug messenger to allocate during construction.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance(const loader& parent, const application_description& app_description,
                   const debug_messenger_description& messenger_description,
                   const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param wsi A window_system to integrate into the debug_instance.
     * @param app_description A description of the application constructing the debug_instance.
     * @param messenger_description A description of a debug messenger to allocate during construction.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance(const loader& parent, const window_system& wsi, const application_description& app_description,
                   const debug_messenger_description& messenger_description,
                   const std::unordered_set<std::string>& requested_layers);

    /// @cond
    debug_instance(const debug_instance& other) = delete;
    debug_instance(debug_instance&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a debug_instance.
     */
    ~debug_instance() = default;

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
