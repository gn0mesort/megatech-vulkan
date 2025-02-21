/**
 * @file opaque_object.hpp
 * @brief Opaque PIMPL Objects
 * @author Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>
 * @copyright AGPL-3.0-or-later
 * @date 2025
 */
#ifndef MEGATECH_VULKAN_CONCEPTS_OPAQUE_OBJECT_HPP
#define MEGATECH_VULKAN_CONCEPTS_OPAQUE_OBJECT_HPP

#include <concepts>
#include <memory>

namespace megatech::vulkan::concepts {

  /**
   * @brief A concept describing an object which exposes a read-only opaque implementation.
   */
  template <typename Type>
  concept readonly_opaque_object = requires (const Type&& ct) {
    { ct.implementation() } -> std::same_as<const typename Type::implementation_type&>;
  };

  /**
   * @brief A concept describing an object which exposes an opaque implementation.
   */
  template <typename Type>
  concept opaque_object = readonly_opaque_object<Type> && requires (Type&& t) {
    { t.implementation() } -> std::same_as<typename Type::implementation_type&>;
  };

  /**
   * @brief A concept describing an object which exposes a shared pointer to a read-only opaque implementation.
   */
  template <typename Type>
  concept readonly_sharable_opaque_object = readonly_opaque_object<Type> && requires (const Type&& ct) {
    { ct.share_implementation() } -> std::same_as<std::shared_ptr<const typename Type::implementation_type>>;
  };

  /**
   * @brief A concept describing an object which exposes a shared pointer to an opaque implementation.
   */
  template <typename Type>
  concept sharable_opaque_object = readonly_sharable_opaque_object<Type> && requires (Type&& t) {
    { t.share_implementation() } -> std::same_as<std::shared_ptr<typename Type::implementation_type>>;
  };

}

#endif
