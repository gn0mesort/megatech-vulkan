#ifndef MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/commands.hpp>

#include "../../error.hpp"
#include "../../version.hpp"

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::instance::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::instance::command::cmd)); \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::device::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::device::command::cmd)); \
  static_assert(true)

#define MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp) \
  do \
  { \
    if (const auto res = (exp); res != VK_SUCCESS) \
    { \
      throw megatech::vulkan::error{ "The command \"" #exp "\" failed.", res }; \
    } \
  } \
  while (0)

namespace megatech::vulkan::internal::base {

  const version MINIMUM_VULKAN_VERSION{ VK_API_VERSION_1_3 };

}

#endif
