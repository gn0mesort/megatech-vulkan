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

namespace megatech::vulkan {

  class application_description;
  class physical_device_description;

}

namespace megatech::vulkan::internal::base {

  class loader_impl;
  class physical_device_validator;

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
     * @param parent A shared_ptr to a read-only loader_impl.
     * @param validator A unique_ptr to a read-only physical_device_validator.
     */
    instance_impl(const std::shared_ptr<const parent_type>& parent,
                  std::unique_ptr<const physical_device_validator>&& validator);

    std::unique_ptr<dispatch::instance::table> m_idt{ };
    std::unique_ptr<const physical_device_validator> m_validator{ };
    std::shared_ptr<const parent_type> m_parent{ };
    VkInstance m_instance{ VK_NULL_HANDLE };
    std::unordered_set<std::string> m_enabled_layers{ };
    std::unordered_set<std::string> m_enabled_extensions{ };
  public:
    using handle_type = VkInstance;

    instance_impl() = delete;
    instance_impl(const std::shared_ptr<const parent_type>& parent,
                  std::unique_ptr<const physical_device_validator>&& validator,
                  const application_description& app_description,
                  const instance_description& description);
    instance_impl(const instance_impl& other) = delete;
    instance_impl(instance_impl&& other) = delete;

    virtual ~instance_impl() noexcept;

    instance_impl& operator=(const instance_impl& rhs) = delete;
    instance_impl& operator=(instance_impl&& rhs) = delete;

    const physical_device_validator& validator() const;

    const dispatch::instance::table& dispatch_table() const;
    handle_type handle() const;

    const parent_type& parent() const;

    const std::unordered_set<std::string>& enabled_layers() const;
    const std::unordered_set<std::string>& enabled_extensions() const;
  };

  class debug_instance_impl final : public instance_impl {
  protected:
    VkDebugUtilsMessengerEXT m_debug_utils_messenger{ VK_NULL_HANDLE };
    std::function<debug_messenger_description::message_sink_fn> m_message_sink{ };
  public:
    debug_instance_impl() = delete;
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        std::unique_ptr<const physical_device_validator>&&,
                        const application_description& app_description,
                        const instance_description& description);
    debug_instance_impl(const std::shared_ptr<const parent_type>& parent,
                        std::unique_ptr<const physical_device_validator>&&,
                        const application_description& app_description,
                        const debug_messenger_description& messenger_description,
                        const instance_description& description);
    debug_instance_impl(const debug_instance_impl& other) = delete;
    debug_instance_impl(debug_instance_impl&& other) = delete;

    ~debug_instance_impl() noexcept;

    debug_instance_impl& operator=(const debug_instance_impl& rhs) = delete;
    debug_instance_impl& operator=(debug_instance_impl&& rhs) = delete;

    void submit_debug_message(const bitmask types, const bitmask severity, const std::string& message) const;
  };

  static_assert(concepts::readonly_child_object<instance_impl>);
  static_assert(concepts::handle_owner<instance_impl>);

}

#endif
