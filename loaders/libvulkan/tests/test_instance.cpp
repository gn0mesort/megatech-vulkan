#include <cinttypes>

#include <ranges>
#include <vector>
#include <string>
#include <iostream>

#include <catch2/catch_all.hpp>

#include <megatech/vulkan.hpp>
#include <megatech/vulkan/dispatch.hpp>
#include <megatech/vulkan/loaders/libvulkan.hpp>
#include <megatech/vulkan/internal/base.hpp>


using megatech::vulkan::bitmask;
using megatech::vulkan::version;
using megatech::vulkan::debug_messenger_description;
using megatech::vulkan::instance;
using megatech::vulkan::debug_instance;

using megatech::vulkan::loaders::libvulkan::loader;
using megatech::vulkan::physical_device_list;

TEST_CASE("Instances should be initializable.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  REQUIRE_NOTHROW(instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } });
  REQUIRE_NOTHROW(instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } }, { "VK_LAYER_KHRONOS_null" } });
}

TEST_CASE("Physical device lists should be initializable from a complete instance object.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
  REQUIRE_NOTHROW(physical_device_list{ inst });
}

TEST_CASE("Physical devices should all have primary queues.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
  auto physical_devices = physical_device_list{ inst };
  for (auto&& physical_device : physical_devices)
  {
    REQUIRE(physical_device.implementation().primary_queue_family_index() != -1);
  }
}

TEST_CASE("Physical devices should be filterable.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
  auto physical_devices = physical_device_list{ inst };
  REQUIRE_NOTHROW(std::views::filter([](auto& v){ return v.supports_rendering(); }));
}

void debug_log(const bitmask, const bitmask severity, const std::string& message) {
  switch (severity)
  {
  case megatech::vulkan::debug_message_severity::verbose_bit:
  case megatech::vulkan::debug_message_severity::info_bit:
  case megatech::vulkan::debug_message_severity::warning_bit:
    std::clog << "[function] " << message << std::endl;
    break;
  case megatech::vulkan::debug_message_severity::error_bit:
    std::cerr << "[function] ERROR: " << message << std::endl;
    break;
  }
}

TEST_CASE("Debug instances should be initializable.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  REQUIRE_NOTHROW(debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } });
  REQUIRE_NOTHROW(debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } }, { "VK_LAYER_KHRONOS_validation" } });
  {
    auto messenger_description = debug_messenger_description{
      [](const bitmask, const bitmask severity, const std::string& message) {
        switch (severity)
        {
        case megatech::vulkan::debug_message_severity::verbose_bit:
        case megatech::vulkan::debug_message_severity::info_bit:
        case megatech::vulkan::debug_message_severity::warning_bit:
          std::clog << "[lambda no-capture] " << message << std::endl;
          break;
        case megatech::vulkan::debug_message_severity::error_bit:
          std::cerr << "[lamdba no-capture] ERROR: " << message << std::endl;
          break;
        }
      }
    };
    REQUIRE_NOTHROW(debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } }, messenger_description,
                                    { "VK_LAYER_KHRONOS_validation" } });
  }
  {
    auto i = 0;
    auto messenger_description = debug_messenger_description{
      [&i](const bitmask, const bitmask severity, const std::string& message) {
        switch (severity)
        {
        case megatech::vulkan::debug_message_severity::verbose_bit:
        case megatech::vulkan::debug_message_severity::info_bit:
        case megatech::vulkan::debug_message_severity::warning_bit:
          std::clog << "[lambda reference-capture] #" << i++ << " " << message << std::endl;
          break;
        case megatech::vulkan::debug_message_severity::error_bit:
          std::cerr << "[lamdba reference-capture] #" << i++ << " " << "ERROR: " << message << std::endl;
          break;
        }
      }
    };
    auto inst = debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } }, messenger_description,
                                { "VK_LAYER_KHRONOS_validation" } };
    inst.submit_debug_message(megatech::vulkan::debug_message_type::general_bit,
                              megatech::vulkan::debug_message_severity::verbose_bit, "Test Message Submission!");
    REQUIRE(i != 0);
  }
  {
    auto messenger_description = debug_messenger_description{ debug_log };
    REQUIRE_NOTHROW(debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } }, messenger_description,
                                    { "VK_LAYER_KHRONOS_validation" } });
  }
}

TEST_CASE("Debug instances should act as if they are regular instances.", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  auto inst = debug_instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
  auto physical_devices = physical_device_list{ inst };
  REQUIRE_NOTHROW(std::views::filter([](auto& v){ return v.supports_rendering(); }));
}

TEST_CASE("Instance ownership chains should ensure parent object lifetimes.", "[instance][loader-libvulkan]") {
  auto* physical_devices = static_cast<physical_device_list*>(nullptr);
  auto loader_ptr = std::weak_ptr<const megatech::vulkan::internal::base::loader_impl>{ };
  auto instance_ptr = std::weak_ptr<const megatech::vulkan::internal::base::instance_impl>{ };
  {
    auto ldr = loader{ };
    loader_ptr = ldr.share_implementation();
    REQUIRE(loader_ptr.use_count() == 1);
    auto inst = instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
    instance_ptr = inst.share_implementation();
    REQUIRE(loader_ptr.use_count() == 2);
    REQUIRE(instance_ptr.use_count() == 1);
    physical_devices = new physical_device_list{ inst };
    REQUIRE(loader_ptr.use_count() == 2);
    REQUIRE(instance_ptr.use_count() >= 1);
  }
  REQUIRE(loader_ptr.use_count() > 0);
  REQUIRE(instance_ptr.use_count() > 0);
  delete physical_devices;
  REQUIRE(loader_ptr.use_count() == 0);
  REQUIRE(instance_ptr.use_count() == 0);
}

TEST_CASE("Physical Devices should never return a queue family index greater than the number of queues available", "[instance][loader-libvulkan]") {
  auto ldr = loader{ };
  auto inst = instance{ ldr, { "test_instance", version{ 0, 1, 0, 0 } } };
  auto physical_devices = physical_device_list{ inst };
  for (const auto& device : physical_devices)
  {
    REQUIRE(device.implementation().primary_queue_family_index() < static_cast<std::int64_t>(device.implementation().queue_family_properties().size()));
    REQUIRE(device.implementation().async_compute_queue_family_index() < static_cast<std::int64_t>(device.implementation().queue_family_properties().size()));
    REQUIRE(device.implementation().async_transfer_queue_family_index() < static_cast<std::int64_t>(device.implementation().queue_family_properties().size()));
  }
}

int main(int argc, char** argv) {
  return Catch::Session{ }.run(argc, argv);
}
