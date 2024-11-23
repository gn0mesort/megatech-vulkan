#include <cinttypes>

#include <vector>
#include <string>

#include <catch2/catch_all.hpp>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch.hpp>

#include <megatech/vulkan/error.hpp>
#include <megatech/vulkan/loader.hpp>
#include <megatech/vulkan/instance.hpp>
#include <megatech/vulkan/physical_devices.hpp>

#include <megatech/vulkan/internal/base/vulkandefs.hpp>
#include <megatech/vulkan/internal/base/loader_impl.hpp>
#include <megatech/vulkan/internal/base/instance_impl.hpp>
#include <megatech/vulkan/internal/base/physical_device_description_impl.hpp>

#include <megatech/vulkan/adaptors/libvulkan/loader.hpp>
#include <megatech/vulkan/adaptors/libvulkan/instance.hpp>

#define VK_DECLARE_INSTANCE_PFN(dt, cmd) MEGATECH_VULKAN_INTERNAL_BASE_DECLARE_INSTANCE_PFN(dt, cmd)
#define VK_CHECK(exp) MEGATECH_VULKAN_INTERNAL_BASE_VK_CHECK(exp)

using megatech::vulkan::version;

using megatech::vulkan::adaptors::libvulkan::loader;
using megatech::vulkan::adaptors::libvulkan::instance;
using megatech::vulkan::physical_device_list;

TEST_CASE("Instances should be initializable.", "[instance][adaptor-libvulkan]") {
  auto ldr = loader{ };
  REQUIRE_NOTHROW(instance{ ldr, "test_instance", version{ 0, 1, 0, 0 } });
  REQUIRE_NOTHROW(instance{ ldr, "test_instance", version{ 0, 1, 0, 0 }, { "VK_LAYER_KHRONOS_null" } });
}

TEST_CASE("Physical device lists should be initializable from a complete instance object.", "[instance][adaptor-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, "test_instance", version{ 0, 1, 0, 0 } };
  REQUIRE_NOTHROW(physical_device_list{ inst });
}

TEST_CASE("Physical device lists created via an instance should be the same as directly listing devices with Vulkan 1.0", "[instance][adaptor-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, "test_instance", version{ 0, 1, 0, 0 } };
  auto physical_devices = physical_device_list{ inst };
  auto vk_physical_devices = std::vector<VkPhysicalDevice>{ };
  {
    auto sz = std::uint32_t{ 0 };
    VK_DECLARE_INSTANCE_PFN(inst.implementation().dispatch_table(), vkEnumeratePhysicalDevices);
    VK_CHECK(vkEnumeratePhysicalDevices(inst.implementation().handle(), &sz, nullptr));
    vk_physical_devices.resize(sz);
    VK_CHECK(vkEnumeratePhysicalDevices(inst.implementation().handle(), &sz, vk_physical_devices.data()));
  }
  REQUIRE(physical_devices.size() == vk_physical_devices.size());
  for (auto i = std::size_t{ 0 }; i < physical_devices.size(); ++i)
  {
    REQUIRE(physical_devices[i].implementation().handle() == vk_physical_devices[i]);
  }
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
