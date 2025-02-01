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

namespace megatech::vulkan::internal::base {

  /**
   * @brief An object that controls the lifetime of the vkGetInstanceProcAddr function pointer.
   * @details Adaptor implementors must extend this class. The derived class must control the lifetime of the
   *          PFN_vkGetInstanceProcAddr value. That is, any library initialization code should live in the derived
   *          constructor and any library deinitialization code should live in the derived destructor.
   */
  class loader_function_owner {
  private:
    PFN_vkGetInstanceProcAddr m_gipa{ };
  protected:
    /**
     * @brief Construct a loader_function_owner.
     * @details This is an inheriting constructor. Derived loader_function_owners must invoke this to set up their
     *          PFN_vkGetInstanceProcAddr.
     * @param gipa The loader function. This must not be null.
     */
    loader_function_owner(const PFN_vkGetInstanceProcAddr gipa);
  public:
    /// @cond
    loader_function_owner() = delete;
    loader_function_owner(const loader_function_owner& other) = delete;
    loader_function_owner(loader_function_owner&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader_function_owner.
     */
    virtual ~loader_function_owner() noexcept = default;

    /// @cond
    loader_function_owner& operator=(const loader_function_owner& rhs) = delete;
    loader_function_owner& operator=(loader_function_owner&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the loader_function_owner's underlying pointer.
     * @return A function pointer to a vkGetInstanceProcAddr function.
     */
    PFN_vkGetInstanceProcAddr pfn() const;
  };

  /**
   * @brief An implementation of megatech::vulkan::loader.
   */
  class loader_impl final {
  private:
    std::shared_ptr<const loader_function_owner> m_pfn_owner{ };
    std::unique_ptr<dispatch::global::table> m_gdt{ };
    std::unordered_set<layer_description> m_available_layers{ };
    std::unordered_map<std::string, std::unordered_set<std::string>> m_available_extensions{ };
  public:
    /// @cond
    loader_impl() = delete;
    /// @endcond

    /**
     * @brief Construct a loader_impl.
     * @param pfn_owner A shared_ptr to a read-only loader_function_owner. This must not be null.
     */
    loader_impl(const std::shared_ptr<const loader_function_owner>& pfn_owner);

    /// @cond
    loader_impl(const loader_impl& other) = delete;
    loader_impl(loader_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader_impl.
     */
    ~loader_impl() noexcept = default;

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
