/// @cond INTERNAL
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_PHYSICAL_DEVICE_DESCRIPTION_IMPL_HPP

#include <unordered_set>
#include <vector>
#include <memory>

#include "../../concepts/child_object.hpp"
#include "../../concepts/handle_owner.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  class instance_impl;

  /**
   * @brief An implementation of a megatech::vulkan::physical_device_description.
   * @details Adaptor implementors that wish to customize the configuration of devices must extend this type and
   *          physical_device_allocator. It should be enough to provide an extended constructor to configure the
   *          requirements of the derived devices.
   */
  class physical_device_description_impl {
  private:
    std::shared_ptr<const instance_impl> m_parent{ };
    VkPhysicalDevice m_handle{ };
    VkPhysicalDeviceProperties m_properties_1_0{ };
    VkPhysicalDeviceVulkan11Properties m_properties_1_1{ };
    VkPhysicalDeviceVulkan12Properties m_properties_1_2{ };
    VkPhysicalDeviceVulkan13Properties m_properties_1_3{ };
    VkPhysicalDeviceFeatures m_features_1_0{ };
    VkPhysicalDeviceVulkan11Features m_features_1_1{ };
    VkPhysicalDeviceVulkan12Features m_features_1_2{ };
    VkPhysicalDeviceVulkan13Features m_features_1_3{ };
    VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR m_dynamic_rendering_local_read_features{ };
    std::vector<VkQueueFamilyProperties> m_queue_family_properties{ };
    std::unordered_set<std::string> m_available_extensions{ };
    int64_t m_primary_queue_family{ -1 };
    int64_t m_async_compute_queue_family{ -1 };
    int64_t m_async_transfer_queue_family{ -1 };
    std::unordered_set<std::string> m_required_extensions{ "VK_KHR_dynamic_rendering_local_read" };
    VkPhysicalDeviceFeatures2 m_required_features{ };
    VkPhysicalDeviceVulkan11Features m_required_features_1_1{ };
    VkPhysicalDeviceVulkan12Features m_required_features_1_2{ };
    VkPhysicalDeviceVulkan13Features m_required_features_1_3{ };
    VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR m_required_dynamic_rendering_local_read_features{ };
  protected:
    /**
     * @brief Manually set the physical_device_description_impl's selected queue families.
     * @details This should only be called by constructors in derived types. Any queue family can be set to -1 to
     *          indicate that it isn't valid.
     * @param primary The primary queue family. This must be less than the number of available queue families.
     * @param compute The asynchronous compute queue family. This must be less than the number of available queue
     *                families.
     * @param transfer The asynchronous transfer queue family. This must be less than the number of available queues.
     */
    void set_queue_families(const std::int64_t primary, const std::int64_t compute, const std::int64_t transfer);

    /**
     * @brief Add a required Vulkan device extension to the physical_device_description_impl's set of required
     *        extensions.
     * @details It is up to adaptor implementors to discover and resolve optional extensions. In short, check if
     *          the optional extension is in available_extensions() and then require it if it is.
     * @param extension The name of the extension to require. Empty strings are ignored.
     */
    void add_required_extension(const std::string& extension);

    /**
     * @brief Add required Vulkan 1.0 features to the physical_device_description_impl.
     * @details Features are always merged. You cannot disable base requirements using this method.
     * @param features A feature object to merge into the base required features.
     */
    void require_1_0_features(const VkPhysicalDeviceFeatures& features);

    /**
     * @brief Add required Vulkan 1.1 features to the physical_device_description_impl.
     * @details Features are always merged. You cannot disable base requirements using this method.
     * @param features A feature object to merge into the base required features.
     */
    void require_1_1_features(const VkPhysicalDeviceVulkan11Features& features);

    /**
     * @brief Add required Vulkan 1.2 features to the physical_device_description_impl.
     * @details Features are always merged. You cannot disable base requirements using this method.
     * @param features A feature object to merge into the base required features.
     */
    void require_1_2_features(const VkPhysicalDeviceVulkan12Features& features);

    /**
     * @brief Add required Vulkan 1.3 features to the physical_device_description_impl.
     * @details Features are always merged. You cannot disable base requirements using this method.
     * @param features A feature object to merge into the base required features.
     */
    void require_1_3_features(const VkPhysicalDeviceVulkan13Features& features);

    /**
     * @brief Add an extended set of Vulkan feature objects to the physical_device_description_impl's required
     *        features.
     * @details This is a strictly non-owning operation. The pointer is appended to the pNext chain of the required
     *          feature object naively. All of the pointed-to objects in the extended chain must have their lifetimes
     *          controlled by the extended class. They must share the same lifetime as the
     *          physical_device_description_impl.
     * @param next A pointer to a pNext chain for VkPhysicalDeviceFeatures2. This must not contain any feature object
     *             already in the chain. This means it cannot contain VkPhysicalDeviceVulkan11Features,
     *             VkPhysicalDeviceVulkan12Features, VkPhysicalDeviceVulkan13Features, or
     *             VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR. It also cannot contain any feature object
     *             that is mutually exclusive with those objects. Passing nullptr will clear the extended pointer
     *             chain.
     */
    void append_extended_feature_chain(void *const next);

    /**
     * @brief Validate the availability of extended features in the physical_device_description_impl.
     * @details Adaptor implementors who need to use append_extended_feature_chain should override this. The
     *          default implementation always returns true. This is called by is_valid.
     * @return True if all of the extended features are available. False otherwise.
     */
    virtual bool has_extended_features() const;
  public:
    using handle_type = VkPhysicalDevice;
    using parent_type = instance_impl;

    /// @cond
    physical_device_description_impl() = delete;
    /// @endcond

    /**
     * @brief Construct a physical_device_description_impl.
     * @param parent A shared_ptr to a read-only instance_impl. This must not be null.
     * @param handle A VkPhysicalDevice handle representing the underlying physical device. This must not be
     *               VK_NULL_HANDLE.
     */
    physical_device_description_impl(std::shared_ptr<const instance_impl> parent, VkPhysicalDevice handle);

    /// @cond
    physical_device_description_impl(const physical_device_description_impl& other) = delete;
    physical_device_description_impl(physical_device_description_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a physical_device_description_impl.
     */
    virtual ~physical_device_description_impl() noexcept = default;

    /// @cond
    physical_device_description_impl& operator=(const physical_device_description_impl& rhs) = delete;
    physical_device_description_impl& operator=(physical_device_description_impl&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the VkPhysicalDevice handle associated with a physical_device_description_impl.
     * @return A VkPhysicalDevice handle.
     */
    handle_type handle() const;

    /**
     * @brief Retrieve the parent instance_impl from a physical_device_description_impl.
     * @return A read-only reference to an instance_impl.
     */
    const parent_type& parent() const;

    /**
     * @brief Retrieve the Vulkan 1.0 properties describing a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceProperties object.
     */
    const VkPhysicalDeviceProperties& properties_1_0() const;

    /**
     * @brief Retrieve the Vulkan 1.1 properties describing a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan11Properties object.
     */
    const VkPhysicalDeviceVulkan11Properties& properties_1_1() const;

    /**
     * @brief Retrieve the Vulkan 1.2 properties describing a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan12Properties object.
     */
    const VkPhysicalDeviceVulkan12Properties& properties_1_2() const;

    /**
     * @brief Retrieve the Vulkan 1.3 properties describing a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan13Properties object.
     */
    const VkPhysicalDeviceVulkan13Properties& properties_1_3() const;


    /**
     * @brief Retrieve the Vulkan 1.0 features available to a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceFeatures object.
     */
    const VkPhysicalDeviceFeatures& features_1_0() const;

    /**
     * @brief Retrieve the Vulkan 1.1 features available to a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan11Features object.
     */
    const VkPhysicalDeviceVulkan11Features& features_1_1() const;

    /**
     * @brief Retrieve the Vulkan 1.2 features available to a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan12Features object.
     */
    const VkPhysicalDeviceVulkan12Features& features_1_2() const;

    /**
     * @brief Retrieve the Vulkan 1.3 features available to a physical_device_description_impl.
     * @return A read-only reference to a VkPhysicalDeviceVulkan13Features object.
     */
    const VkPhysicalDeviceVulkan13Features& features_1_3() const;

    /**
     * @brief Retrieve the extensions available to a physical_device_description_impl.
     * @return A read-only reference to a set of Vulkan extensions.
     */
    const std::unordered_set<std::string>& available_extensions() const;

    /**
     * @brief Retrieve descriptions of the queue families available to a physical_device_description_impl.
     * @return A read-only reference to an array of VkQueueFamilyProperties objects.
     */
    const std::vector<VkQueueFamilyProperties>& queue_family_properties() const;

    /**
     * @brief Retrieve the index of the primary queue family selected by a physical_device_description_impl.
     * @return An integer in the range [0, queue_family_properties().size()) if a primary queue is available. -1
     *         otherwise.
     */
    int64_t primary_queue_family_index() const;

    /**
     * @brief Retrieve the index of the asynchronous compute queue family selected by a
     *        physical_device_description_impl.
     * @return An integer in the range [0, queue_family_properties().size()) if an asynchronous compute queue is
     *         available. -1 otherwise.
     */
    int64_t async_compute_queue_family_index() const;

    /**
     * @brief Retrieve the index of the asynchronous transfer queue family selected by a
     *        physical_device_description_impl.
     * @return An integer in the range [0, queue_family_properties().size()) if an asynchronous transfer queue is
     *         available. -1 otherwise.
     */
    int64_t async_transfer_queue_family_index() const;

    /**
     * @brief Retrieve the properties of a physical_device_description_impl's primary queue family.
     * @return A the properties of the primary queue family.
     * @throws error If there isn't a primary queue family.
     */
    const VkQueueFamilyProperties& primary_queue_family_properties() const;

    /**
     * @brief Retrieve the properties of a physical_device_description_impl's asynchronous compute queue family.
     * @return A the properties of the asynchronous compute queue family.
     * @throws error If there isn't an asynchronous compute queue family.
     */
    const VkQueueFamilyProperties& async_compute_queue_family_properties() const;

    /**
     * @brief Retrieve the properties of a physical_device_description_impl's asynchronous transfer queue family.
     * @return A the properties of the asynchronous transfer queue family.
     * @throws error If there isn't an asynchronous transfer queue family.
     */
    const VkQueueFamilyProperties& async_transfer_queue_family_properties() const;

    /**
     * @brief Determine if a physical_device_description_impl meets the minimum requirements for Megatech-Vulkan.
     * @return True if the device is valid. False otherwise.
     */
    bool is_valid() const;

    /**
     * @brief Retrieve the extensions required by a physical_device_description_impl.
     * @return A read-only reference to a set of required Vulkan extensions.
     */
    const std::unordered_set<std::string>& required_extensions() const;

    /**
     * @brief Retrieve the features required by a physical_device_description_impl.
     * @details This is suitable to attach to a VkDeviceCreateInfo's pNext chain. The returned reference shares a
     *          lifetime with the physical_device_description_impl that owns it.
     * @return A read-only reference to a set of required Vulkan features.
     */
    const VkPhysicalDeviceFeatures2& required_features() const;
  };

  static_assert(concepts::readonly_child_object<physical_device_description_impl>);
  static_assert(concepts::handle_owner<physical_device_description_impl>);

}

#endif
