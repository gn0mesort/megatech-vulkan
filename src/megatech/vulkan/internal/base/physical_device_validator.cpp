#include "megatech/vulkan/internal/base/physical_device_validator.hpp"

#include "megatech/vulkan/version.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

namespace megatech::vulkan::internal::base {

  bool physical_device_validator::has_core_requirements(const physical_device_description& physical_device) const noexcept {
    return true;
  }

  bool physical_device_validator::has_extended_requirements(const physical_device_description&) const noexcept {
    return true;
  }

  bool physical_device_validator::is_valid(const physical_device_description& physical_device) const noexcept {
    return has_core_requirements(physical_device) && has_extended_requirements(physical_device);
  }

  bool physical_device_validator::operator()(const physical_device_description& physical_device) const noexcept {
    return is_valid(physical_device);
  }

}
