# Megatech-Vulkan

Megatech-Vulkan is my experimental Vulkan utility and rendering library. My goal is to build a largely portable
and configurable interface to Vulkan that eliminates most of the boilerplate. Rather than building a complete
multimedia framework, I want to keep Megatech-Vulkan focused on the core features of Vulkan (i.e., compute and
rendering).

This is very much a work in progress.

## What Works

Right now, Megatech-Vulkan is primarily useful as a way to easily bootstrap a working Vulkan device without a ton of
boilerplate. The basic approach looks like this:

```cpp
#include <megatech/vulkan.hpp>
#include <megatech/vulkan/loaders/libvulkan.hpp>

namespace mv = megatech::vulkan;

// An adaptor is needed to ensure the correct features and extensions are enabled.
// The libvulkan adaptor dynamically links libvulkan and enables only the core features required by Megatech-Vulkan.
// It doesn't support any window system integration.
namespace libvulkan = mv::loaders::libvulkan;

int main() {
  // Initialize a Vulkan instance.
  auto instance = mv::instance{ libvulkan::loader{ }, { "My application", { 0, 1, 0, 0 } } };
  // Enumerate physical devices and ensure that there is a valid device
  auto physical_devices = mv::physical_device_list{ instance };
  if (physical_devices.empty())
  {
    return 1;
  }
  // Initialize the first valid Vulkan device.
  auto device = mv::device{ physical_devices.front() };
  return 0;
}
```

Although they're not exposed directly, you can get the underlying Vulkan handles for these objects through the
internal API (i.e., from `include/megatech/vulkan/internal/base.hpp`).

## What Doesn't Work Yet

Really everything interesting isn't included here. I'd like to implement the following features in the future:

- Device resource management
- A directed acyclic graph based scheduling system for device operations
- Support for presentation features
- Adaptors for common window system frameworks (SDL2, GLFW3, and maybe X11)

## Dependencies

Megatech-Vulkan should be largely dependency free. To use it you will need:

- Megatech-Vulkan Dispatch (included as a submodule)
- Megatech-Assertions (included as a submodule)
- A valid vulkan.h header file

If you want to use the libvulkan adaptor you will also need

- libvulkan

## Licensing

Copyright (C) 2024 Alexander Rothman <[gnomesort@megate.ch](mailto:gnomesort@megate.ch)>

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more
details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see
<[https://www.gnu.org/licenses/](https://www.gnu.org/licenses/)>.
