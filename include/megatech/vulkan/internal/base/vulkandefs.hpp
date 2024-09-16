#ifndef MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/commands.hpp>

#include "../../error.hpp"

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  if (!cmd) \
  { \
    throw error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::instance::command::cmd)); \
  if (!cmd) \
  { \
    throw error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)


#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::device::command::cmd)); \
  if (!cmd) \
  { \
    throw error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp) \
  do \
  { \
    if (const auto res = (exp); res != VK_SUCCESS) \
    { \
      throw error{ "The command \"" #exp "\" failed.", res }; \
    } \
  } \
  while (0)

#endif
