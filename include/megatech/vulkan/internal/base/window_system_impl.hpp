/// @cond INTERNAL
/**
 * @file window_system_impl.hpp
 * @brief Window System Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_WINDOW_SYSTEM_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_WINDOW_SYSTEM_IMPL_HPP

#include <cinttypes>

#include <ranges>
#include <unordered_set>

#include "../../window_system.hpp"

#include "vulkandefs.hpp"

namespace megatech::vulkan::internal::base {

  /**
   * @brief An implementation of megatech::vulkan::window_system.
   */
  class window_system_impl {
  private:
    std::unordered_set<std::string> m_required_extensions{ };
  protected:
    /**
     * @brief Construct a window_system_impl.
     * @details This is a deferred initialization constructor.
     */
    window_system_impl() = default;

    /**
     * @brief Add a range of extenions to the window sytem's list of required extensions.
     * @tparam Range The range type to accept.
     * @param r A Range of values that can be inserted into a set of strings.
     */
    template <std::ranges::forward_range Range>
    void add_required_extensions(Range&& r);
  public:
    /// @cond
    window_system_impl(const window_system_impl& other) = delete;
    window_system_impl(window_system_impl&& other) = delete;
    /// @endcond

    /**
     * @brief Destroy a window_system_impl.
     */
    virtual ~window_system_impl() noexcept = default;

    /// @cond
    window_system_impl& operator=(const window_system_impl& rhs) = delete;
    window_system_impl& operator=(window_system_impl&& rhs) = delete;
    /// @endcond


    /**
     * @brief Retrieve the set of Vulkan extensions required by the window_system_impl.
     * @return A read-only reference to a set of required Vulkan WSI extensions.
     */
    const std::unordered_set<std::string>& required_extensions() const;

    /**
     * @brief Determine whether or not the window_system_impl supports checking for presentation support without a
     *        Vulkan surface.
     * @details As far as I know, every Vulkan WSI extension supports this. However, some frameworks (e.g., SDL2) do
     *          not. The result is that it may not be possible to check presentation support using an instance alone.
     * @return True if the window_system_impl can check presentation support independently. False otherwise.
     */
    virtual bool can_check_presentation_support() const;

    /**
     * @brief Determine if a given physical device queue family supports presentation operations.
     * @return True if the queue family supports presentation operations. False otherwise.
     * @throw megatech::vulkan::error If the window system does not support checking presentation support without a
     *                                surface.
     */
    virtual bool is_presentation_supported(const VkPhysicalDevice physical_device,
                                           const std::uint32_t queue_family) const;
  };

  template <std::ranges::forward_range Range>
  void window_system_impl::add_required_extensions(Range&& r) {
    m_required_extensions.insert(std::begin(r), std::end(r));
  }

}

#endif
/// @endcond
