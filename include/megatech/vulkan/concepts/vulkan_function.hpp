#ifndef MEGATECH_VULKAN_CONCEPTS_VULKAN_FUNCTION_HPP
#define MEGATECH_VULKAN_CONCEPTS_VULKAN_FUNCTION_HPP

#include <cstddef>

#include <concepts>
#include <tuple>
#include <type_traits>

#if defined(MEGATECH_VULKAN_INCLUDE_VULKAN_H) && __has_include(<vulkan/vulkan.h>)
  #include <vulkan/vulkan.h>
#endif

namespace megatech::vulkan::concepts::internal::base {

  template <typename Type>
  struct function_prototype;

  template <typename Result, typename... Args>
  struct function_prototype<Result (*)(Args...)> final {
    using result_type = Result;
    using argument_tuple_type = std::tuple<Args...>;
    static constexpr std::size_t argument_count{ sizeof...(Args) };
  };

}

#ifdef VK_VERSION_1_0
namespace megatech::vulkan::concepts {

  template <typename Type>
  concept vulkan_function = requires {
    requires std::is_pointer_v<Type> && std::is_function_v<std::remove_pointer_t<Type>>;
    requires (std::same_as<typename internal::base::function_prototype<Type>::result_type, void> ||
              std::same_as<typename internal::base::function_prototype<Type>::result_type, VkResult> ||
              std::same_as<typename internal::base::function_prototype<Type>::result_type, PFN_vkVoidFunction>);
  };

}
#endif

#endif
