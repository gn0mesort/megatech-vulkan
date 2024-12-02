#ifndef MEGATECH_VULKAN_CONCEPTS_HANDLE_OWNER_HPP
#define MEGATECH_VULKAN_CONCEPTS_HANDLE_OWNER_HPP

#include <concepts>

namespace megatech::vulkan::concepts {

  template <typename Type>
  concept handle_owner = requires (Type&& t, const Type&& ct) {
    { t.handle() } -> std::same_as<typename Type::handle_type>;
    { ct.handle() } -> std::same_as<typename Type::handle_type>;
  };

}

#endif
