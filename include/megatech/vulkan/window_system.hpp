/**
 * @file window_system.hpp
 * @brief Vulkan Window Systems
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_WINDOW_SYSTEM_HPP
#define MEGATECH_VULKAN_WINDOW_SYSTEM_HPP

#include <memory>

#include "concepts/opaque_object.hpp"

namespace megatech::vulkan::internal::base {

  class window_system_impl;

}

namespace megatech::vulkan {

  /**
   * @brief An object for loading and managing platform-specific window system functionality.
   */
  class window_system {
  public:
    /**
     * @brief The internal implementation type of the window_system.
     */
    using implementation_type = internal::base::window_system_impl;
  private:
    std::shared_ptr<implementation_type> m_impl;
  protected:
    /**
     * @brief Construct a window_system.
     * @details This is an inheriting constructor.
     * @param impl A shared_ptr pointer to the implementation. This must not be null.
     */
    explicit window_system(const std::shared_ptr<implementation_type>& impl);
  public:
    /// @cond
    window_system() = delete;
    window_system(const window_system& other) = delete;
    window_system(window_system&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a window_system.
     */
    virtual ~window_system() noexcept = default;

    /// @cond
    window_system& operator=(const window_system& rhs) = delete;
    window_system& operator=(window_system&& rhs) = delete;
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

  static_assert(concepts::opaque_object<window_system>);
  static_assert(concepts::readonly_sharable_opaque_object<window_system>);

}

#endif
