dependencies = [
  catch2_dep,
  vulkan_dep,
  megatech_vulkan_dispatch_dep,
  megatech_vulkan_dep,
  megatech_vulkan_adaptor_libvulkan_dep
]
test_loader_exe = executable('test-loader', files('test_loader.cpp'), dependencies: dependencies)
test_instance_exe = executable('test-instance', files('test_instance.cpp'), dependencies: dependencies)
test_device_exe = executable('test-device', files('test_device.cpp'), dependencies: dependencies)

test('Loader', test_loader_exe, suite: 'adaptor-libvulkan')
test('Instance', test_instance_exe, suite: 'adaptor-libvulkan')
test('Device', test_device_exe, suite: 'adaptor-libvulkan')
