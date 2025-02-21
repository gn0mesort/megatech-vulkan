#include <iostream>

#include <megatech/vulkan.hpp>
#include <megatech/vulkan/adaptors/libvulkan.hpp>

namespace mv = megatech::vulkan;
namespace mval = megatech::vulkan::adaptors::libvulkan;

template <typename OutputStream>
OutputStream& insert_severity(OutputStream&& out, const mv::bitmask severity) {
  out << "[";
  switch (severity)
  {
  case mv::debug_message_severity::error_bit:
    out << "Error";
    break;
  case mv::debug_message_severity::warning_bit:
    out << "Warning";
    break;
  case mv::debug_message_severity::info_bit:
    out << "Info";
    break;
  case mv::debug_message_severity::verbose_bit:
    out << "Verbose";
    break;
  }
  return out << "]: ";
}

void debug_sink(const mv::bitmask, const mv::bitmask severity, const std::string& message) {
  insert_severity(std::clog, severity) << message << "\n";
}

int main() {
  auto debugger = mv::debug_messenger_description{ debug_sink };
  auto inst = mv::debug_instance{ mval::loader{ },  { }, debugger, { "VK_LAYER_KHRONOS_validation" } };
  auto physical_devices = mv::physical_device_list{ inst };
  if (physical_devices.empty())
  {
    return 1;
  }
  auto dev = mv::device{ physical_devices.front() };
  return 0;
}
