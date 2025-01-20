#include "megatech/vulkan/internal/base/physical_device_validator.hpp"

#include "megatech/vulkan/error.hpp"
#include "megatech/vulkan/version.hpp"

#include "megatech/vulkan/internal/base/vulkandefs.hpp"
#include "megatech/vulkan/internal/base/instance_impl.hpp"
#include "megatech/vulkan/internal/base/physical_device_description_impl.hpp"

#define DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)

namespace megatech::vulkan::internal::base {

  bool physical_device_validator::has_core_requirements(const physical_device_description& physical_device) const noexcept {
    try
    {
      const auto& impl = physical_device.implementation();
      const auto good_version = version{ impl.properties_1_0().apiVersion } >= version{ 0, 1, 3, 0 };
      const auto has_primary_queue = impl.primary_queue_family_index() != -1;
      DECLARE_INSTANCE_PFN(impl.parent().dispatch_table(), vkGetPhysicalDeviceFeatures2);
      auto features2 = VkPhysicalDeviceFeatures2{ };
      features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
      auto local_read_features = VkPhysicalDeviceDynamicRenderingLocalReadFeaturesKHR{ };
      local_read_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_LOCAL_READ_FEATURES_KHR;
      features2.pNext = &local_read_features;
      vkGetPhysicalDeviceFeatures2(impl.handle(), &features2);
      const auto has_dynamic_rendering = impl.features_1_3().dynamicRendering &&
                                         local_read_features.dynamicRenderingLocalRead &&
                                         impl.available_extensions().contains("VK_KHR_dynamic_rendering_local_read");
      return good_version && has_primary_queue && has_dynamic_rendering;
    }
    catch (const error&)
    {
      return false;
    }
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
