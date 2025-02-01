/**
 * @file handle_owner.hpp
 * @brief Vulkan Handle Owners
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_CONCEPTS_HANDLE_OWNER_HPP
#define MEGATECH_VULKAN_CONCEPTS_HANDLE_OWNER_HPP

#include <concepts>

namespace megatech::vulkan::concepts {

  /**
   * @brief A concept describing an object that exposes a Vulkan handle.
   */
  template <typename Type>
  concept handle_owner = requires (Type&& t, const Type&& ct) {
    { t.handle() } -> std::same_as<typename Type::handle_type>;
    { ct.handle() } -> std::same_as<typename Type::handle_type>;
  };

}

#endif
