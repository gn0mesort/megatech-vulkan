/// @cond INTERNAL
/**
 * @file instance_impl.hpp
 * @brief Instance Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP

#include <cinttypes>

#include <memory>
#include <unordered_set>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../debug_messenger_description.hpp"
#include "../../instance.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan {

  class application_description;

}

namespace megatech::vulkan::internal::base {

  class loader_impl;
  class physical_device_description_impl;

  /**
   * @brief The implementation of a megatech::vulkan::instance.
   */
  class instance_impl : public std::enable_shared_from_this<instance_impl> {
  public:
    /**
     * @brief The parent object type that must be used to initialize an instance_impl.
     */
    using parent_type = loader_impl;
  private:
    std::unique_ptr<dispatch::instance::table> m_idt{ };
    std::shared_ptr<const parent_type> m_parent{ };
    std::unordered_set<std::string> m_enabled_layers{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  protected:
    /**
     * @brief Construct an instance_impl.
     * @details This is a deferred initialization constructor.
     * @param parent A read-only shared_ptr to the parent loader implementation.
     */
    explicit instance_impl(const std::shared_ptr<const parent_type>& parent);

    /**
     * @brief Create an instance_impl's underlying VkInstance.
     * @details This should only ever be called from a constructor.
     * @param app_description A description of the application to be passed down to Vulkan.
     * @param required_layers A set of layers that will be enabled in the created instance.
     * @param required_extensions A set of extensions that will be enabled in the created instance.
     * @param next A valid Vulkan pNext pointer chain. This must be compatible with VkInstanceCreateInfo. nullptr is
     *             a valid input.
     */
    void create_instance(const application_description& app_description,
                         const std::unordered_set<std::string>& required_layers,
                         const std::unordered_set<std::string>& required_extensions,
                         const void *const next);

    /**
     * @brief Destroy an instance_impl's underlying VkInstance and set the handle to VK_NULL_HANDLE.
     * @details This should only ever be called from a destructor.
     */
    void destroy_instance() noexcept;
  public:
    /**
     * @brief The type of an instance_impl's owned Vulkan handle.
     */
    using handle_type = VkInstance;

    /**
     * @brief Construct an instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    instance_impl(const std::shared_ptr<const parent_type>& parent,
                  const application_description& app_description,
                  const std::unordered_set<std::string>& requested_layers);

    /// @cond
    instance_impl() = delete;
    instance_impl(const instance_impl& other) = delete;
    instance_impl(instance_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy an instance_impl.
     */
    virtual ~instance_impl() noexcept;

    /// @cond
    instance_impl& operator=(const instance_impl& rhs) = delete;
    instance_impl& operator=(instance_impl&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the instance_impl's dispatch table.
     * @return A read-only reference to an instance dispatch table.
     */
    const dispatch::instance::table& dispatch_table() const;

    /**
     * @brief Retrieve the instance_impl's Vulkan handle.
     * @return The VkInstance handle owned by the instance_impl.
     */
    handle_type handle() const;

    /**
     * @brief Retrieve the instance_impl's parent loader_impl.
     * @return A read-only reference to the parent loader_impl.
     */
    const parent_type& parent() const;

    /**
     * @brief Retrieve the instance_impl's enabled layers.
     * @return a read-only reference to a set of Vulkan layers.
     */
    const std::unordered_set<std::string>& enabled_layers() const;

    /**
     * @brief Retrieve the instance_impl's enabled extensions.
     * @return A read-only reference to a set of Vulkan extensions.
     */
    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  /**
   * @brief The implementation of a megatech::vulkan::debug_instance.
   */
  class debug_instance_impl final : public instance_impl {
  private:
    VkDebugUtilsMessengerEXT m_debug_utils_messenger{ VK_NULL_HANDLE };
    std::function<debug_messenger_description::message_sink_fn> m_message_sink{ };
  protected:
    /**
     * @brief Construct a debug_instance_impl.
     * @details This is a deferred initialization constructor.
     * @param parent A read-only shared_ptr to the parent loader.
     */
    explicit debug_instance_impl(const std::shared_ptr<const parent_type>& parent);

    /**
     * @brief Construct a debug_instance_impl.
     * @details This is a deferred initialization constructor.
     * @param parent A read-only shared_ptr to the parent loader.
     * @param messenger_description A description of a debug messenger. The sink function is taken and copied in this
     *                              constructor.
     */
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        const debug_messenger_description& messenger_description);

    /**
     * @brief Create a debug_instance_impl's underlying VkDebugUtilsMessengerEXT.
     * @details This should only be called from a constructor.
     * @param info A prepacked info structure describing the VkDebugUtilsMessengerEXT to create.
     */
    void create_debug_messenger(const VkDebugUtilsMessengerCreateInfoEXT& info);

    /**
     * @brief Destroy a debug_instance_impl's underlying VkDebugUitlsMessengerEXT.
     * @details This should only be called from a destructor.
     */
    void destroy_debug_messenger() noexcept;
  public:
    /// @cond
    debug_instance_impl() = delete;
    /// @endcond

    /**
     * @brief Construct a debug_instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        const application_description& app_description,
                        const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct a debug_instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param messenger_description A description of a debug messenger that will be constructed alongside the
     *                              underlying Vulkan instance. Dependencies of the messenger description must have
     *                              a longer lifetime than the underlying instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     */
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        const application_description& app_description,
                        const debug_messenger_description& messenger_description,
                        const std::unordered_set<std::string>& requested_layers);

    /// @cond
    debug_instance_impl(const debug_instance_impl& other) = delete;
    debug_instance_impl(debug_instance_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a debug_instance_impl.
     */
    ~debug_instance_impl() noexcept;

    /// @cond
    debug_instance_impl& operator=(const debug_instance_impl& rhs) = delete;
    debug_instance_impl& operator=(debug_instance_impl&& rhs) = delete;
    /// @endcond

    /**
     * @brief Submit a custom debug message to the debug_instance_impl's message sink.
     * @param types A bitmask of message types.
     * @param severity The severity of the message. This must be a single bit.
     * @param message The message to deliver.
     */
    void submit_debug_message(const bitmask types, const bitmask severity, const std::string& message) const;
  };

  static_assert(concepts::readonly_child_object<instance_impl>);
  static_assert(concepts::handle_owner<instance_impl>);

}

#endif
/// @endcond
