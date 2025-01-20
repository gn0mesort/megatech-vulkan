#ifndef MEGATECH_VULKAN_CONCEPTS_BUILDER_OBJECT_HPP
#define MEGATECH_VULKAN_CONCEPTS_BUILDER_OBJECT_HPP

#include <concepts>

namespace megatech::vulkan::concepts {

  template <typename Type, typename Result>
  concept builder_object = requires (Type&& t) {
    { t.build() } -> std::same_as<Result>;
  };

}

#endif
