dependencies = [
  catch2_dep,
  vulkan_dep,
  megatech_vulkan_dispatch_dep,
  megatech_vulkan_dep,
  megatech_vulkan_adaptor_libvulkan_dep
]
test_loader_exe = executable('test-loader', files('test_loader.cpp'), dependencies: dependencies)

test('Loader', test_loader_exe, suite: 'adaptor-libvulkan')
