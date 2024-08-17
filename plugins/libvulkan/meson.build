dependencies = [
  vulkan_dep,
  megatech_vulkan_dispatch_dep,
  megatech_vulkan_dep
]
includes = [
  include_directories('include')
]
sources = [
  files('src/megatech/vulkan/plugins/libvulkan/loader.cpp'),
  files('src/megatech/vulkan/plugins/libvulkan/internal/base/loader_impl.cpp')
]
megatech_vulkan_plugin_libvulkan_lib = library('@0@-plugin-libvulkan'.format(meson.project_name()), sources,
                                               include_directories: includes, dependencies: dependencies,
                                               version: version)
megatech_vulkan_plugin_libvulkan_dep = declare_dependency(link_with: megatech_vulkan_plugin_libvulkan_lib,
                                                          include_directories: includes,
                                                          dependencies: megatech_vulkan_dep)
