/// @cond INTERNAL
#ifndef MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP
#define MEGATECH_VULKAN_INTERNAL_BASE_VULKANDEFS_HPP

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch/commands.hpp>

#include "../../error.hpp"
#include "../../version.hpp"

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN
 * @brief Declare a global-level Vulkan command function pointer.
 * @param dt A reference to a megatech::vulkan::dispatch::global::table object.
 * @param cmd The name of the command to declare. For example, vkGetInstanceProcAddr. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN_NO_THROW
 * @brief Declare a global-level Vulkan command function pointer.
 * @details This variant is safe for noexcept contexts like destructors.
 * @param dt A reference to a megatech::vulkan::dispatch::global::table object.
 * @param cmd The name of the command to declare. For example, vkGetInstanceProcAddr. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_GLOBAL_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  static_assert(true)


/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN
 * @brief Declare an instance-level Vulkan command function pointer.
 * @param dt A reference to a megatech::vulkan::dispatch::instance::table object.
 * @param cmd The name of the command to declare. For example, vkDestroyInstance. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::instance::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW
 * @brief Declare an instance-level Vulkan command function pointer.
 * @details This variant is safe for noexcept contexts like destructors.
 * @param dt A reference to a megatech::vulkan::dispatch::instance::table object.
 * @param cmd The name of the command to declare. For example, vkDestroyInstance. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::instance::command::cmd)); \
  static_assert(true)

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN
 * @brief Declare a device-level Vulkan command function pointer.
 * @param dt A reference to a megatech::vulkan::dispatch::device::table object.
 * @param cmd The name of the command to declare. For example, vkDestroyDevice. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::device::command::cmd)); \
  if (!cmd) \
  { \
    throw megatech::vulkan::error{ "Failed to resolve the command \"" #cmd "\"." }; \
  } \
  static_assert(true)

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN_NO_THROW
 * @brief Declare a device-level Vulkan command function pointer.
 * @details This variant is safe for noexcept contexts like destructors.
 * @param dt A reference to a megatech::vulkan::dispatch::device::table object.
 * @param cmd The name of the command to declare. For example, vkDestroyDevice. This isn't a string.
 */
#define MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_DEVICE_PFN_NO_THROW(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::device::command::cmd)); \
  static_assert(true)

/**
 * @def MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK
 * @brief Check a Vulkan expression.
 * @details This will execute the input expression and then check if the result is VK_SUCCESS. If it is, an error will
 *          be thrown. Otherwise, the check passes silently.
 * @param exp The expression to check. This must resolve to a value comparable to VK_SUCCESS.
 */
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

  /**
   * @brief The minimum version of the Vulkan API that must be supported.
   */
  const version MINIMUM_VULKAN_VERSION{ VK_API_VERSION_1_3 };

}

#endif
/// @endcond
