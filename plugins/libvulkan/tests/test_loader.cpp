#include <cinttypes>

#include <vector>

#include <catch2/catch_all.hpp>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan.hpp>
#include <megatech/vulkan/plugins/libvulkan.hpp>

TEST_CASE("Loaders should be default initializable.", "[loader][plugin-libvulkan]") {
  REQUIRE_NOTHROW(megatech::vulkan::plugins::libvulkan::loader{ });
}

TEST_CASE("Loaders should resolve all of the global Vulkan commands without error.", "[loader][plugin-libvulkan]") {
  using megatech::vulkan::plugins::libvulkan::loader;
  using namespace megatech::vulkan::command_tags;
  const auto ldr = loader{ };
  REQUIRE_NOTHROW(ldr.resolve<vk_get_instance_proc_addr>());
  REQUIRE_NOTHROW(ldr.resolve<vk_enumerate_instance_version>());
  REQUIRE_NOTHROW(ldr.resolve<vk_enumerate_instance_layer_properties>());
  REQUIRE_NOTHROW(ldr.resolve<vk_enumerate_instance_extension_properties>());
  REQUIRE_NOTHROW(ldr.resolve<vk_create_instance>());
}

TEST_CASE("Retrieving the loader dispatch table should not be an error.", "[loader][plugin-libvulkan]") {
  using megatech::vulkan::plugins::libvulkan::loader;
  const auto ldr = loader{ };
  REQUIRE_NOTHROW(ldr.dispatch_table());
}

TEST_CASE("Loaders should not return a nullptr when sharing their implementation", "[loader][plugin-libvulkan]") {
  using megatech::vulkan::plugins::libvulkan::loader;
  const auto ldr = loader{ };
  REQUIRE(ldr.share_implementation() != nullptr);
}

TEST_CASE("Loaders should be able to retrieve a list of Vulkan layers for the client.", "[loader][plugin-libvulkan]") {
  using megatech::vulkan::plugins::libvulkan::loader;
  using namespace megatech::vulkan::command_tags;
  const auto ldr = loader{ };
  const auto ldr_layers = ldr.available_layers();
  const auto vkEnumerateInstanceLayerProperties = ldr.resolve<vk_enumerate_instance_layer_properties>();
  auto vk_layers = std::vector<VkLayerProperties>{ };
  {
    auto sz = std::uint32_t{ };
    if (const auto res = vkEnumerateInstanceLayerProperties(&sz, nullptr); res != VK_SUCCESS)
    {
      FAIL("Retrieving the list of Vulkan layers failed.");
    }
    vk_layers.resize(sz);
    if (const auto res = vkEnumerateInstanceLayerProperties(&sz, vk_layers.data()); res != VK_SUCCESS)
    {
      FAIL("Retrieving the list of Vulkan layers failed.");
    }
  }
  REQUIRE(ldr_layers.size() == vk_layers.size());
  for (auto i = std::uint32_t{ 0 }; i < ldr_layers.size(); ++i)
  {
    REQUIRE(ldr_layers[i].name() == vk_layers[i].layerName);
    REQUIRE(ldr_layers[i].description() == vk_layers[i].description);
    REQUIRE(static_cast<std::uint32_t>(ldr_layers[i].specification_version()) == vk_layers[i].specVersion);
    REQUIRE(static_cast<std::uint32_t>(ldr_layers[i].implementation_version()) == vk_layers[i].implementationVersion);
  }
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
