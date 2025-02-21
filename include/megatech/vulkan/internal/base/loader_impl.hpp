/// @cond INTERNAL
/**
 * @file loader_impl.hpp
 * @brief Loader Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP

#include <memory>
#include <unordered_map>

#include <megatech/vulkan/dispatch/tables.hpp>

#include "../../loader.hpp"
#include "../../layer_description.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan {

  class application_description;
  class debug_messenger_description;

}

namespace megatech::vulkan::internal::base {

  class instance_impl;
  class debug_instance_impl;

  /**
   * @brief An implementation of megatech::vulkan::loader.
   */
  class loader_impl : public std::enable_shared_from_this<loader_impl> {
  private:
    std::unique_ptr<dispatch::global::table> m_gdt{ };
    std::unordered_set<layer_description> m_available_layers{ };
    std::unordered_map<std::string, std::unordered_set<std::string>> m_available_extensions{ };
  protected:
    /**
     * @brief Construct a loader_impl.
     * @details This is a deferred initialization constructor.
     */
    loader_impl() = default;

    /**
     * @brief Construct a loader_impl.
     * @details This is a simple solution for loader implementations that do not need specialized behavior at
     *          construction to acquire vkGetInstanceProcAddr. Adaptors that acquire the loader function prior to
     *          run-time, or as part of program loading can use this constructor to simplify implementation.
     * @param pfn A pointer to the base vkGetInstanceProcAddr function for the loader to use. This must not be null.
     */
    explicit loader_impl(const PFN_vkGetInstanceProcAddr pfn);

    /**
     * @brief Set the loader_impl's vkGetInstanceProcAddr function pointer.
     * @details This should only be called once in a constructor. It should never be called elsewhere. Adaptors are
     *          responsible for the lifetime of the function pointer.
     * @param pfn A pointer to the base vkGetInstanceProcAddr function for the loader to use. This must not be null.
     */
    void set_loader_pfn(const PFN_vkGetInstanceProcAddr pfn);
  public:
    /// @cond
    loader_impl(const loader_impl& other) = delete;
    loader_impl(loader_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader_impl.
     */
    virtual ~loader_impl() noexcept = default;

    /// @cond
    loader_impl& operator=(const loader_impl& rhs) = delete;
    loader_impl& operator=(loader_impl&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the loader_impl's dispatch table.
     * @return A read-only reference to a global dispatch table.
     */
    const dispatch::global::table& dispatch_table() const;

    /**
     * @brief Retrieve the layers available to Vulkan instances from a loader_impl.
     * @details The underlying layers are queried only once during construction. It's possible that the layers
     *          available to an implementation could change between queries. However, Megatech-Vulkan doesn't support
     *          this. Restart your application instead.
     * @return A read-only reference to a set of layer_descriptions.
     */
    const std::unordered_set<layer_description>& available_layers() const;

    /**
     * @brief Retrieve the extensions available to all Vulkan instances from a loader_impl.
     * @return A read-only reference to a set of Vulkan extension names.
     */
    const std::unordered_set<std::string>& available_instance_extensions() const;

    /**
     * @brief Retrieve the extensions available to Vulkan instances with the specified layer enabled from a
     *        loader_impl.
     * @param layer The name of the layer to query available extensions from. Passing the empty string will query
     *              extensions that are available to all instances. The layer name must be a valid layer name returned
     *              from available_layers() or the empty string.
     * @return A read-only reference to a set of Vulkan extension names.
     */
    const std::unordered_set<std::string>& available_instance_extensions(const std::string& layer) const;

    /**
     * @brief Resolve an instance_impl.
     * @details This method resolves the type of created instance implementations. The default behavior is to return
     *          a default megatech::vulkan::internal::base::instance_impl. Adaptors that define extended instance types
     *          should override this to return the extended implementation type instead.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     * @return A pointer to a new instance_impl. The caller is responsible for managing the pointer's lifetime.
     */
    virtual instance_impl* resolve_instance(const application_description& app_description,
                                            const std::unordered_set<std::string>& requested_layers) const;

    /**
     * @brief Resolve a debug_instance_impl.
     * @details This method resolves the type of created debug_instance implementations. The default behavior is to
     *          return a default megatech::vulkan::internal::base::debug_instance_impl. Adaptors that define extended
     *          instance types should override this to return the extended implementation type instead.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     * @return A pointer to a new debug_instance_impl. The caller is responsible for managing the pointer's lifetime.
     */
    virtual debug_instance_impl* resolve_debug_instance(const application_description& app_description,
                                                        const std::unordered_set<std::string>& requested_layers) const;

    /**
     * @brief Resolve a debug_instance_impl.
     * @details This method resolves the type of created debug_instance implementations. The default behavior is to
     *          return a default megatech::vulkan::internal::base::debug_instance_impl. Adaptors that define extended
     *          instance types should override this to return the extended implementation type instead.
     * @param app_description A description of the client application to pass to the underlying Vulkan implementation.
     * @param messenger_description A description of a debug messenger that will be constructed alongside the
     *                              underlying Vulkan instance. Dependencies of the messenger description must have
     *                              a longer lifetime than the underlying instance.
     * @param requested_layers A set of layers to enable. Each layer is only enabled if it is available. If a layer
     *                         is unavailable it is ignored.
     * @return A pointer to a new debug_instance_impl. The caller is responsible for managing the pointer's lifetime.
     */
    virtual debug_instance_impl* resolve_debug_instance(const application_description& app_description,
                                                        const debug_messenger_description& messenger_description,
                                                        const std::unordered_set<std::string>& requested_layers) const;
  };

}

#endif
/// @endcond
