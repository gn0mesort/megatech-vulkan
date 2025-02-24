dependencies = [
  vulkan_dep,
  megatech_assertions_dep,
  megatech_vulkan_dispatch_dep,
  megatech_vulkan_dep
]
includes = [
  include_directories('include')
]
sources = [
  files('src/megatech/vulkan/loaders/libvulkan/loader.cpp'),
  files('src/megatech/vulkan/loaders/libvulkan/internal/base/loader_impl.cpp')
]
megatech_vulkan_loader_libvulkan_lib = library('@0@-loader-@1@'.format(meson.project_name(), 'libvulkan'), sources,
                                                include_directories: includes, dependencies: dependencies,
                                                version: version)
megatech_vulkan_loader_libvulkan_dep = declare_dependency(link_with: megatech_vulkan_loader_libvulkan_lib,
                                                           include_directories: includes)
subdir('tests')
doc_inputs += [ meson.current_source_dir() / 'include', meson.current_source_dir() / 'documentation' ]
doc_strips += [ meson.current_source_dir() / 'include' ]
