#ifndef MEGATECH_VULKAN_LOADER_HPP
#define MEGATECH_VULKAN_LOADER_HPP

#include <memory>
#include <unordered_set>

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class loader_impl;

}

namespace megatech::vulkan {

  class layer_description;

  /**
   * @brief An object for loading global Vulkan functionality.
   * @details There is no Vulkan equivalent of the loader type. Instead, loaders take the form of function pointers.
   *          In Megatech-Vulkan, loaders are the base object on which everything else depends. They are responsible
   *          for loading Vulkan's most basic features.
   */
  class loader {
  public:
    using implementation_type = internal::base::loader_impl;
  private:
    std::shared_ptr<implementation_type> m_impl;
  protected:
    explicit loader(const std::shared_ptr<implementation_type>& impl);
  public:
    /// @cond
    loader() = delete;
    loader(const loader& other) = delete;
    loader(loader&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a loader.
     * @details Destroying a loader may or may not release its underlying resources. If the loader is in use, it will
     *          remain valid until the last user is destroyed. This prevents invalid destruction orders.
     */
    virtual ~loader() noexcept = default;

    /// @cond
    loader& operator=(const loader& rhs) = delete;
    loader& operator=(loader&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve a set of available Vulkan layers.
     * @details This set is loaded exactly once, when the loader is created. The Vulkan specification allows this
     *          set to change at run-time, however Megatech-Vulkan does not.
     * @return A set of layer_description objects, each representing an available Vulkan layer that can be explicitly
     *         enabled during instance creation.
     */
    const std::unordered_set<layer_description>& available_layers() const;

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

  static_assert(concepts::opaque_object<loader>);
  static_assert(concepts::readonly_sharable_opaque_object<loader>);

}

#endif
