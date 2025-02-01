/**
 * @file child_object.hpp
 * @brief Child Objects
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_CONCEPTS_CHILD_OBJECT_HPP
#define MEGATECH_VULKAN_CONCEPTS_CHILD_OBJECT_HPP

#include <concepts>

namespace megatech::vulkan::concepts {

  /**
   * @brief A concept describing an object that exposes a read-only reference to a parent object.
   */
  template <typename Type>
  concept readonly_child_object = requires (const Type&& ct) {
    { ct.parent() } -> std::same_as<const typename Type::parent_type&>;
  };

  /**
   * @brief A concept describing an object that exposes a reference to a parent object.
   */
  template <typename Type>
  concept child_object = requires (Type&& t) {
    { t.parent() } -> std::same_as<typename Type::parent_type&>;
  };

}

#endif
