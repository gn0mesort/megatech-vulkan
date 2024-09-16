dependencies = [
  vulkan_dep,
  megatech_vulkan_dep,
  megatech_vulkan_plugin_libvulkan_dep
]

if get_option('benchmarks').allowed()
  dependencies += dependency('catch2', required: get_option('benchmarks'))

  benchmark('Resolver Benchmark', executable('test-benchmark-resolvers', files('benchmark_resolvers.cpp'), dependencies: dependencies),
            verbose: true, timeout: 0)
endif
