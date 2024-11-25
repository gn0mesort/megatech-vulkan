#include <cinttypes>

#include <ranges>
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

TEST_CASE("Physical devices should all have primary queues.", "[instance][adaptor-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, "test_instance", version{ 0, 1, 0, 0 } };
  auto physical_devices = physical_device_list{ inst };
  for (auto&& physical_device : physical_devices)
  {
    REQUIRE(physical_device.implementation().primary_queue_family_index() != -1);
  }
}

TEST_CASE("Physical devices should be filterable.", "[instance][adaptor-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, "test_instance", version{ 0, 1, 0, 0 } };
  auto physical_devices = physical_device_list{ inst };
  REQUIRE_NOTHROW(std::views::filter([](auto& v){ return v.supports_rendering(); }));
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
