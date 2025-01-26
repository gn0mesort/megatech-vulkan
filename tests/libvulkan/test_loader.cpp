#include <cinttypes>

#include <vector>
#include <string>

#include <catch2/catch_all.hpp>


#include <megatech/vulkan.hpp>
#include <megatech/vulkan/dispatch.hpp>
#include <megatech/vulkan/adaptors/libvulkan.hpp>
#include <megatech/vulkan/internal/base.hpp>


#define DECLARE_GLOBAL_PFN(dt, cmd) \
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

TEST_CASE("Loaders should be default initializable.", "[loader][adaptor-libvulkan]") {
  REQUIRE_NOTHROW(loader{ });
}

TEST_CASE("Loaders should have a shareable implementation pointer.", "[loader][adaptor-libvulkan]") {
  const auto ldr = loader{ };
  REQUIRE(ldr.share_implementation() != nullptr);
}

TEST_CASE("Loaders should be able to retrieve a list of Vulkan layers for the client.", "[loader][adaptor-libvulkan]") {
  using megatech::vulkan::version;
  const auto ldr = loader{ };
  auto vk_layers = std::vector<VkLayerProperties>{ };
  {
    auto sz = std::uint32_t{ };
    DECLARE_GLOBAL_PFN(ldr.implementation().dispatch_table(), vkEnumerateInstanceLayerProperties);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, nullptr));
    vk_layers.resize(sz);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&sz, vk_layers.data()));
  }
  REQUIRE(vk_layers.size() == ldr.available_layers().size());
  for (auto i = std::uint32_t{ 0 }; i < vk_layers.size(); ++i)
  {
    REQUIRE(ldr.available_layers().contains(vk_layers[i].layerName));
  }
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
