#ifndef MEGATECH_VULKAN_CONCEPTS_CHILD_OBJECT_HPP
#define MEGATECH_VULKAN_CONCEPTS_CHILD_OBJECT_HPP

#include <concepts>

namespace megatech::vulkan::concepts {

  template <typename Type>
  concept readonly_child_object = requires (const Type&& ct) {
    { ct.parent() } -> std::same_as<const typename Type::parent_type&>;
  };

  template <typename Type>
  concept child_object = requires (Type&& t) {
    { t.parent() } -> std::same_as<typename Type::parent_type&>;
  };

}

#endif
