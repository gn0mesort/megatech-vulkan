/// @cond INTERNAL
/**
 * @file window_system_impl.hpp
 * @brief Window System Implementation
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_LOADER_IMPL_HPP

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

    const std::unordered_set<std::string>& required_extensions() const;
    virtual bool can_check_presentation_support() const;
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
