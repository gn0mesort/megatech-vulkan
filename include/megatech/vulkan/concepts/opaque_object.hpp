#ifndef MEGATECH_VULKAN_CONCEPTS_HPP
#define MEGATECH_VULKAN_CONCEPTS_HPP
#include <concepts>
#include <memory>

namespace megatech::vulkan::concepts {

  template <typename Type>
  concept readonly_opaque_object = requires (const Type&& ct) {
    { ct.implementation() } -> std::same_as<const typename Type::implementation_type&>;
  };

  template <typename Type>
  concept opaque_object = readonly_opaque_object<Type> && requires (Type&& t) {
    { t.implementation() } -> std::same_as<typename Type::implementation_type&>;
  };

  template <typename Type>
  concept readonly_sharable_opaque_object = readonly_opaque_object<Type> && requires (const Type&& ct) {
    { ct.share_implementation() } -> std::same_as<std::shared_ptr<const typename Type::implementation_type>>;
  };

  template <typename Type>
  concept sharable_opaque_object = readonly_sharable_opaque_object<Type> && requires (Type&& t) {
    { t.share_implementation() } -> std::same_as<std::shared_ptr<typename Type::implementation_type>>;
  };

}

#endif
