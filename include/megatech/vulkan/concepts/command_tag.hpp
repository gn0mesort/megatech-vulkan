#ifndef MEGATECH_VULKAN_CONCEPTS_COMMAND_TAG_HPP
#define MEGATECH_VULKAN_CONCEPTS_COMMAND_TAG_HPP
#include <concepts>

#include "vulkan_function.hpp"

#ifdef VK_VERSION_1_0
namespace megatech::vulkan::concepts {

  template <typename Type>
  concept command_tag = requires (Type&& t) {
    requires vulkan_function<typename Type::function_pointer_type>;
    { Type::name() } -> std::same_as<const char*>;
  };

}
#endif

#endif
