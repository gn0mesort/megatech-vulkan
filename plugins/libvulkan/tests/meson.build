if get_option('tests').allowed()
  dependencies = [
    megatech_vulkan_dep,
    megatech_vulkan_plugin_libvulkan_dep,
    catch2_dep,
    vulkan_dep
  ]
  test('Plugin "libvulkan" Loader', executable('test-loader', files('test_loader.cpp'), dependencies: dependencies))
endif
