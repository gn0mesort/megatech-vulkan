#ifndef MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP
#define MEGATECH_VULKAN_ADAPTORS_LIBVULKAN_INSTANCE_HPP

#include <unordered_set>

#include <megatech/vulkan/instance.hpp>

namespace megatech::vulkan {

  class version;
  class application_description;
  class debug_messenger_description;

}

namespace megatech::vulkan::adaptors::libvulkan {

  class loader;

  /**
   * @brief A Vulkan instance.
   */
  class instance final : public megatech::vulkan::instance {
  public:
    /// @cond
    instance() = delete;
    /// @endcond

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param requested_layers A set of requested layers that the instance should enable. Layers that are unavailable
     *                         are ignored.
     */
    instance(const loader& parent, const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application.
     */
    instance(const loader& parent, const application_description& app_description);

    /**
     * @brief Construct an instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application.
     * @param requested_layers A set of requested layers that the instance should enable. Layers that are unavailable
     *                         are ignored.
     */
    instance(const loader& parent, const application_description& app_description,
             const std::unordered_set<std::string>& requested_layers);

    /// @cond
    instance(const instance& other) = delete;
    instance(instance&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy an instance.
     */
    ~instance() noexcept = default;

    /// @cond
    instance& operator=(const instance& rhs) = delete;
    instance& operator=(instance&& rhs) = delete;
    /// @endcond
  };


  /**
   * @brief A Vulkan instance with debugging instrumentation.
   */
  class debug_instance final : public megatech::vulkan::debug_instance {
  public:
    /// @cond
    debug_instance() = delete;
    /// @endcond

    /**
     * @brief Construct a debug_instance.
     * @param parent The parent loader object.
     * @param requested_layers A set of requested layers that the instance should enable. Layers that are unavailable
     *                         are ignored.
     */
    debug_instance(const loader& parent, const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct an debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application.
     */
    debug_instance(const loader& parent, const application_description& app_description);

    /**
     * @brief Construct an debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application.
     * @param requested_layers A set of requested layers that the instance should enable. Layers that are unavailable
     *                         are ignored.
     */
    debug_instance(const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers);

    /**
     * @brief Construct an debug_instance.
     * @param parent The parent loader object.
     * @param app_description A description of the application.
     * @param requested_layers A set of requested layers that the instance should enable. Layers that are unavailable
     *                         are ignored.
     * @param messenger_description A description of a debug messenger. The messenger will be constructed internally,
     *                              anything that the messenger's sink function depends on, therefore, needs to have a
     *                              longer lifetime than the instance.
     */
    debug_instance(const loader& parent, const application_description& app_description,
                   const std::unordered_set<std::string>& requested_layers,
                   const debug_messenger_description& messenger_description);

    /// @cond
    debug_instance(const debug_instance& other) = delete;
    debug_instance(debug_instance&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a debug_instance.
     */
    ~debug_instance() noexcept = default;

    /// @cond
    debug_instance& operator=(const debug_instance& rhs) = delete;
    debug_instance& operator=(debug_instance&& rhs) = delete;
    /// @endcond
  };

}

#endif
