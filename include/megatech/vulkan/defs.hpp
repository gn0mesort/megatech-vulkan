#ifndef MEGATECH_VULKAN_DEFS_HPP
#define MEGATECH_VULKAN_DEFS_HPP

#define MEGATECH_VULKAN_AVAILABLE (1)

#define MEGATECH_VULKAN_ENFORCE_CONCEPT(a) \
  static_assert(requires { requires (a); })

#endif
