project(
  'megatech-vulkan',
  [ 'cpp' ],
  meson_version: '>=1.4.0',
  version: '0.1.0',
  license: 'AGPL-3.0-or-later',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
    'b_pie=true',
'cpp_args=-fstrict-aliasing -Wstrict-aliasing=3'
  ]
)
version = meson.project_version()
version_major = version.split('.')[0]
version_minor = version.split('.')[1]
version_patch = version.split('.')[2]
pymod = import('python')
python3 = pymod.find_installation('python3')
has_tools = run_command(python3, '-c',
'''
import megatech.vulkan.applications
''', check: false)
if has_tools.returncode() != 0
  error('Please ensure that all Python requirements are installed.')
endif
vulkan_dep = dependency('vulkan')
catch2_dep = disabler()
if get_option('tests').allowed() or get_option('benchmarks').allowed()
  catch2_dep = dependency('catch2')
endif
megatech_vulkan_dispatch_dep = dependency('megatech-vulkan-dispatch',
                                          fallback: [ 'megatech-vulkan-dispatch', 'megatech_vulkan_dispatch_dep' ])
megatech_assertions_dep = dependency('megatech-assertions',
                                     fallback: [ 'megatech-assertions', 'megatech_assertions_dep' ])
dependencies = [
  vulkan_dep.partial_dependency(includes: true),
  megatech_vulkan_dispatch_dep,
  megatech_assertions_dep
]
includes = [
  include_directories('include')
]
config = configuration_data()
config.set_quoted('CONFIG_VULKAN_ENGINE_NAME', meson.project_name())
config.set('CONFIG_VULKAN_ENGINE_VERSION_MAJOR', version_major)
config.set('CONFIG_VULKAN_ENGINE_VERSION_MINOR', version_minor)
config.set('CONFIG_VULKAN_ENGINE_VERSION_PATCH', version_patch)
compiler = meson.get_compiler('cpp')
if compiler.get_id() == 'gcc'
  config.set('CONFIG_COMPILER_GCC', 1)
elif compiler.get_id() == 'clang'
  config.set('CONFIG_COMPILER_CLANG', 1)
elif compiler.get_id() == 'msvc'
  config.set('CONFIG_COMPILER_MSVC', 1)
else
  config.set('CONFIG_COMPILER_UNKNOWN', 1)
endif
config_header = configure_file(input: 'generated/include/config.hpp.in', output: '@BASENAME@', configuration: config)
sources = [
  files('src/megatech/vulkan/error.cpp', 'src/megatech/vulkan/version.cpp',
        'src/megatech/vulkan/application_description.cpp', 'src/megatech/vulkan/debug_messenger_description.cpp',
        'src/megatech/vulkan/layer_description.cpp', 'src/megatech/vulkan/loader.cpp',
        'src/megatech/vulkan/instance.cpp', 'src/megatech/vulkan/physical_devices.cpp',
        'src/megatech/vulkan/device.cpp'),
  files('src/megatech/vulkan/internal/base/loader_impl.cpp',
        'src/megatech/vulkan/internal/base/instance_impl.cpp',
        'src/megatech/vulkan/internal/base/physical_device_description_impl.cpp',
        'src/megatech/vulkan/internal/base/device_impl.cpp'),
  config_header
]
megatech_vulkan_lib = library(meson.project_name(), sources, include_directories: includes,
                              dependencies: dependencies, version: version)
megatech_vulkan_dep = declare_dependency(link_with: megatech_vulkan_lib, include_directories: includes)
dependencies = [
  vulkan_dep,
  megatech_vulkan_dispatch_dep,
  megatech_vulkan_dep
]
sources = [
  files('src/megatech/vulkan/adaptors/libvulkan/loader.cpp'),
  files('src/megatech/vulkan/adaptors/libvulkan/internal/base/loader_impl.cpp')
]
megatech_vulkan_adaptor_libvulkan_lib = library('@0@-adaptor-@1@'.format(meson.project_name(), 'libvulkan'), sources,
                                                include_directories: includes, dependencies: dependencies,
                                                version: version)
megatech_vulkan_adaptor_libvulkan_dep = declare_dependency(link_with: megatech_vulkan_adaptor_libvulkan_lib,
                                                           include_directories: includes)
subdir('tests')
subdir('examples')
doxygen = find_program('doxygen', disabler: true)
doc_env = environment()
doc_env.set('DOXYGEN_PROJECT_NAME', meson.project_name())
doc_env.set('DOXYGEN_PROJECT_NUMBER', meson.project_version())
doc_env.set('DOXYGEN_OUTPUT_DIRECTORY', meson.current_build_dir() / 'documentation')
readme = files('README.md')
doc_inputs = [ meson.current_source_dir() / 'include', config_header.full_path(),
               readme[0].full_path(), meson.current_source_dir() / 'documentation' ]
doc_env.set('DOXYGEN_USE_MDFILE_AS_MAINPAGE', readme[0].full_path())
doc_env.set('DOXYGEN_INPUT', ' '.join(doc_inputs))
doc_strips = [ meson.current_build_dir(), meson.current_source_dir() / 'include' ]
doc_env.set('DOXYGEN_STRIP_FROM_PATH', ' '.join(doc_strips))
extra_css = files('documentation/extra.css')[0]
doc_env.set('DOXYGEN_HTML_EXTRA_STYLESHEET', extra_css.full_path())
doc_env.set('DOXYGEN_ENABLED_SECTIONS', ' '.join(get_option('enabled_doxygen_sections')))
doc_defines = [ 'VK_VERSION_1_0=1', '__DOXYGEN__=1' ]
doc_env.set('DOXYGEN_PREDEFINED', ' '.join(doc_defines))
docs = custom_target(output: 'documentation', build_always_stale: true,
                     command: [ doxygen, files('Doxyfile') ], env: doc_env)
