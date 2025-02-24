#include <iostream>

#include <catch2/catch_all.hpp>

#include <megatech/vulkan.hpp>
#include <megatech/vulkan/loaders/libvulkan.hpp>

using megatech::vulkan::bitmask;
using megatech::vulkan::version;
using megatech::vulkan::debug_messenger_description;
using megatech::vulkan::debug_instance;
using megatech::vulkan::physical_device_list;
using megatech::vulkan::device;

using megatech::vulkan::loaders::libvulkan::loader;

TEST_CASE("Devices should be initializable.", "[device][loader-libvulkan]") {
  auto ldr = loader{ };
  auto validation_error_count = 0;
  auto messenger_description = debug_messenger_description{
    [&validation_error_count](const bitmask, const bitmask severity, const std::string& message) {
      switch (severity)
      {
      case megatech::vulkan::debug_message_severity::verbose_bit:
      case megatech::vulkan::debug_message_severity::info_bit:
      case megatech::vulkan::debug_message_severity::warning_bit:
        std::clog << message << std::endl;
        break;
      case megatech::vulkan::debug_message_severity::error_bit:
        std::cerr << "ERROR: " << message << std::endl;
        ++validation_error_count;
        break;
      }
    }
  };
  auto inst = debug_instance{ ldr, { "test_device", version{ 0, 1, 0, 0 } }, messenger_description,
                              { "VK_LAYER_KHRONOS_validation" } };
  auto physical_devices = physical_device_list{ inst };
  REQUIRE_NOTHROW(device{ physical_devices.front() });
  REQUIRE(validation_error_count == 0);
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
