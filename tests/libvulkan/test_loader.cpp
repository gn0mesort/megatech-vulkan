#include <cinttypes>

#include <vector>
#include <string>

#include <catch2/catch_all.hpp>

#define VK_NO_PROTOTYPES (1)
#include <vulkan/vulkan.h>

#include <megatech/vulkan/dispatch.hpp>

#include <megatech/vulkan/loader.hpp>
#include <megatech/vulkan/layer_description.hpp>

#include <megatech/vulkan/internal/base/loader_impl.hpp>

#include <megatech/vulkan/adaptors/libvulkan.hpp>

#define VK_DECLARE_GLOBAL_PFN(dt, cmd) \
  const auto cmd = *reinterpret_cast<const PFN_##cmd*>((dt).get(megatech::vulkan::dispatch::global::command::cmd)); \
  do \
  { \
    if (!cmd) \
    { \
      FAIL("The command \"" #cmd "\" was a nullptr."); \
    } \
  } \
  while (0)

#define VK_CHECK(exp) \
  do \
  { \
    if ((exp) != VK_SUCCESS) \
    { \
      FAIL("The expression \"" #exp "\" failed."); \
    } \
  } \
  while (0)

using megatech::vulkan::adaptors::libvulkan::loader;

TEST_CASE("Loaders should be default initializable.", "[loader][plugin-libvulkan]") {
  REQUIRE_NOTHROW(loader{ });
}

TEST_CASE("Loaders should be explicitly copy initializable.", "[loader][plugin-libvulkan]") {
  auto ldr = loader{ };
  REQUIRE_NOTHROW(loader{ ldr });
}

TEST_CASE("Loaders should have a shareable implementation pointer.", "[loader][plugin-libvulkan]") {
  const auto ldr = loader{ };
  REQUIRE(ldr.share_implementation() != nullptr);
}

TEST_CASE("Loaders should be able to retrieve a list of Vulkan layers for the client.", "[loader][plugin-libvulkan]") {
  using megatech::vulkan::version;
  const auto ldr = loader{ };
  const auto ldr_layers = ldr.available_layers();
  auto vk_layers = std::vector<VkLayerProperties>{ };
  {
    auto sz = std::uint32_t{ };
    VK_DECLARE_GLOBAL_PFN(ldr.implementation().dispatch_table(), vkEnumerateInstanceLayerProperties);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, nullptr));
    vk_layers.resize(sz);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, vk_layers.data()));
  }
  REQUIRE(vk_layers.size() == ldr_layers.size());
  for (auto i = std::uint32_t{ 0 }; i < vk_layers.size(); ++i)
  {
    REQUIRE(version{ vk_layers[i].specVersion } == ldr_layers[i].specification_version());
    REQUIRE(vk_layers[i].implementationVersion == ldr_layers[i].implementation_version());
    REQUIRE(std::string{ vk_layers[i].layerName } == ldr_layers[i].name());
    REQUIRE(std::string{ vk_layers[i].description } == ldr_layers[i].description());
  }
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
