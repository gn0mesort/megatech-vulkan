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
  class loader_impl {
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
     * @details This is a simple solution for implementations that do not need any specialized management of their
     *          loader function. If you need to explicitly manage the lifetime of your loader function, use
     *          ::store_loader_pfn instead.
     * @param pfn A pointer to the base vkGetInstanceProcAddr function for the loader_impl to use. This must not be
     *            null.
     */
    explicit loader_impl(const PFN_vkGetInstanceProcAddr pfn);

    /**
     * @brief Store the loader_impl's vkGetInstanceProcAddr function pointer.
     * @details This must be called during construction. The stored pointer must remain valid for the lifetime of the
     *          loader_impl, and it is the caller's responsibility to manage it. It is unsafe to release the pointer
     *          outside of the loader_impl's destructor.
     * @param pfn A pointer to the base vkGetInstanceProcAddr function for the loader_impl to use. This must not be
     *            null.
     */
    void store_loader_pfn(const PFN_vkGetInstanceProcAddr pfn);

    /**
     * @brief Retrieve the loader_impl's vkGetInstaceProcAddr function pointer.
     * @details This is useful to prevent needing two copies of the vkGetInstanceProcAddr pointer. Instead of keeping
     *          a separate record, you can use this to retrieve the previously stored pointer during destruction.
     * @return The pointer that the loader_impl was initialized with.
     */
    PFN_vkGetInstanceProcAddr retrieve_loader_pfn() const;
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
  };

}

#endif
/// @endcond
