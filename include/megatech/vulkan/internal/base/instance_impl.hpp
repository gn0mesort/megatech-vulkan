/// @cond INTERNAL
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_INSTANCE_IMPL_HPP

#include <cinttypes>

#include <memory>
#include <unordered_set>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../defs.hpp"
#include "../../debug_messenger_description.hpp"
#include "../../instance.hpp"

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

#include "vulkandefs.hpp"
#include "physical_device_allocator.hpp"

namespace megatech::vulkan {

  class application_description;

}

namespace megatech::vulkan::internal::base {

  class loader_impl;

  /**
   * @brief A desctiption of a Vulkan instance.
   * @details instance_descriptions replace VkInstanceCreateInfo. More so than VkDeviceCreateInfo, most of the
   *          configuration of VkInstanceCreateInfo isn't appropriate for client applications. This exposes layers
   *          and extensions only.
   */
  class instance_description final {
  private:
    std::unordered_set<std::string> m_requested_layers{ };
    std::unordered_set<std::string> m_requested_extensions{ };
    std::unordered_set<std::string> m_required_extensions{ };
  public:
    /**
     * @brief Construct an instance_description.
     */
    instance_description() = default;

    /**
     * @brief Construct an instance_description.
     * @param requested_layers A set of requested (optional) Vulkan layers to enable if they are available.
     * @param requested_extensions A set of requested (optional) Vulkan instance extensions to enable if they are
     *                             available.
     * @param required_extensions A set of required (mandatory) Vulkan instance extensions to enable.
     */
    instance_description(const std::unordered_set<std::string>& requested_layers,
                         const std::unordered_set<std::string>& requested_extensions,
                         const std::unordered_set<std::string>& required_extensions);

    /**
     * @brief Copy an instance_description.
     * @param other The instance_description to copy.
     */
    instance_description(const instance_description& other) = default;

    /**
     * @brief Move an instance_description.
     * @param other The instance_description to move.
     */
    instance_description(instance_description&& other) = default;

    /**
     * @brief Destroy an instance_description.
     */
    ~instance_description() noexcept = default;

    /**
     * @brief Copy-assign an instance_description.
     * @param rhs The instance_description to copy.
     * @return A reference to the copied-to instance_description.
     */
    instance_description& operator=(const instance_description& other) = default;

    /**
     * @brief Move-assign an instance_description.
     * @param rhs The instance_description to move.
     * @return A reference to the moved-to instance_description.
     */
    instance_description& operator=(instance_description&& other) = default;

    /**
     * @brief Retrieve an instance_description's set of requested layers.
     * @return A read-only reference to the set of requested layers.
     */
    const std::unordered_set<std::string>& requested_layers() const;

    /**
     * @brief Retrieve an instance_description's set of requested instance extensions.
     * @return A read-only reference to the set of requested instance extensions.
     */
    const std::unordered_set<std::string>& requested_extensions() const;

    /**
     * @brief Retrieve an instance_description's set of required instance extensions.
     * @return A read-only reference to the set of required instance extensions.
     */
    const std::unordered_set<std::string>& required_extensions() const;
  };

  /**
   * @brief The implementation of a megatech::vulkan::instance.
   */
  class instance_impl {
  public:
    using parent_type = loader_impl;
  protected:
    /**
     * @brief Construct an instance_impl.
     * @details This is an inheritance only constructor that sets up the parent and validator. This should only be
     *          invoked as part of a derived constructor.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param allocator A read-only unique_ptr to a physical_device_allocator object. This should be the matching
     *                  allocator to the type of physical_device_description_impl used by your adaptor. If your
     *                  adaptor doesn't provide a derived physical_device_description_impl, you can use the default
     *                  implementation. This must not be null.
     */
    instance_impl(const std::shared_ptr<const parent_type>& parent,
                  std::unique_ptr<const class physical_device_allocator>&& allocator);

    std::unique_ptr<dispatch::instance::table> m_idt{ };
    std::unique_ptr<const class physical_device_allocator> m_physical_device_allocator{ };
    std::shared_ptr<const parent_type> m_parent{ };
    VkInstance m_instance{ VK_NULL_HANDLE };
    std::unordered_set<std::string> m_enabled_layers{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  public:
    using handle_type = VkInstance;

    /// @cond
    instance_impl() = delete;
    /// @endcond

    /**
     * @brief Construct an instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param allocator A read-only unique_ptr to a physical_device_allocator object. This should be the matching
     *                  allocator to the type of physical_device_description_impl used by your adaptor. If your
     *                  adaptor doesn't provide a derived physical_device_description_impl, you can use the default
     *                  implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param description A description of the instance layers and extensions to enable.
     */
    instance_impl(const std::shared_ptr<const parent_type>& parent,
                  std::unique_ptr<const class physical_device_allocator>&& allocator,
                  const application_description& app_description,
                  const instance_description& description);

    /// @cond
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
     * @brief Retrieve the instance_impl's physical_device_allocator.
     * @return A read-only reference to a physical_device_allocator object.
     */
    const class physical_device_allocator& physical_device_allocator() const;

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
  protected:
    VkDebugUtilsMessengerEXT m_debug_utils_messenger{ VK_NULL_HANDLE };
    std::function<debug_messenger_description::message_sink_fn> m_message_sink{ };
  public:
    /// @cond
    debug_instance_impl() = delete;
    /// @endcond

    /**
     * @brief Construct a debug_instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param allocator A read-only unique_ptr to a physical_device_allocator object. This should be the matching
     *                  allocator to the type of physical_device_description_impl used by your adaptor. If your
     *                  adaptor doesn't provide a derived physical_device_description_impl, you can use the default
     *                  implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param description A description of the instance layers and extensions to enable.
     */
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        std::unique_ptr<const class physical_device_allocator>&&,
                        const application_description& app_description,
                        const instance_description& description);

    /**
     * @brief Construct a debug_instance_impl.
     * @param parent A read-only shared_ptr to the parent loader's implementation. This must not be null.
     * @param allocator A read-only unique_ptr to a physical_device_allocator object. This should be the matching
     *                  allocator to the type of physical_device_description_impl used by your adaptor. If your
     *                  adaptor doesn't provide a derived physical_device_description_impl, you can use the default
     *                  implementation. This must not be null.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param messenger_description A description of a debug messenger that will be constructed alongside the
     *                              underlying Vulkan instance. Dependencies of the messenger description must have
     *                              a longer lifetime than the underlying instance.
     * @param description A description of the instance layers and extensions to enable.
     */
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        std::unique_ptr<const class physical_device_allocator>&&,
                        const application_description& app_description,
                        const debug_messenger_description& messenger_description,
                        const instance_description& description);

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
