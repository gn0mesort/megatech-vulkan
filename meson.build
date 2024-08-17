project(
  'megatech-vulkan',
  [ 'cpp' ],
  version: '0.0.0-alpha1',
  license: 'AGPL-3.0-or-later',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
    'b_pie=true',
    'cpp_args=-fstrict-aliasing -Wstrict-aliasing=3'
  ]
)
version = meson.project_version().split('-')[0]
pymod = import('python')
python3 = pymod.find_installation('python3')
has_tools = run_command(python3, '-c',
'''
import megatech.vulkan.applications
''', check: false)
if has_tools.returncode() != 0
  error('Please ensure that all Python requirements are installed.')
endif
extensions = ','.join(get_option('extensions'))
template_arguments = [ ]
generator = [ find_program('dispatch-table-generator'), '-o@OUTPUT@', '--api=@0@'.format(get_option('api')),
              '--api-version=@0@'.format(get_option('api_version')), '--extensions=@0@'.format(extensions) ]
if not get_option('generator_warnings').allowed()
  generator += '--quiet'
endif
specification = get_option('specification')
if specification != ''
  generator += '--specification=@0@'.format(specification)
endif
if not get_option('deprecated_vulkan_features').allowed()
  generator += '--no-enable-deprecated'
endif
headers = [ ]
subdir('generated/include')
vulkan_dep = dependency('vulkan')
megatech_vulkan_dispatch_dep = dependency('megatech-vulkan-dispatch',
                                          fallback: [ 'megatech-vulkan-dispatch', 'megatech_vulkan_dispatch_dep' ])
dependencies = [
  vulkan_dep.partial_dependency(includes: true),
  megatech_vulkan_dispatch_dep
]
includes = [
  include_directories('include')
]
sources = [
  files('src/megatech/vulkan/error.cpp', 'src/megatech/vulkan/loader.cpp'),
  files('src/megatech/vulkan/internal/base/loader_impl.cpp'),
  headers
]
megatech_vulkan_lib = library(meson.project_name(), sources, include_directories: includes,
                              dependencies: dependencies, version: version)
megatech_vulkan_dep = declare_dependency(link_with: megatech_vulkan_lib, include_directories: includes,
                                         sources: headers)
subdir('plugins')
doxygen = find_program('doxygen', disabler: true)
doc_env = environment()
doc_env.set('DOXYGEN_PROJECT_NAME', meson.project_name())
doc_env.set('DOXYGEN_PROJECT_NUMBER', meson.project_version())
doc_env.set('DOXYGEN_OUTPUT_DIRECTORY', meson.current_build_dir() / 'documentation')
doc_inputs = [ meson.current_source_dir() / 'include', meson.current_source_dir() / 'generated',
               files('README.md')[0].full_path(), meson.current_source_dir() / 'documentation' ]
doc_env.set('DOXYGEN_USE_MDFILE_AS_MAINPAGE', doc_inputs[2])
foreach header : headers
  doc_inputs += [ header.full_path() ]
endforeach
doc_env.set('DOXYGEN_INPUT', ' '.join(doc_inputs))
doc_excludes = [ meson.current_source_dir() / 'include/megatech/vulkan/internal' ]
doc_strips = [ meson.current_build_dir() / 'generated/include', meson.current_build_dir() / 'generated/src',
               meson.current_source_dir() / 'include' ]
doc_env.set('DOXYGEN_STRIP_FROM_PATH', ' '.join(doc_strips))
extra_css = files('documentation/extra.css')[0]
doc_env.set('DOXYGEN_HTML_EXTRA_STYLESHEET', extra_css.full_path())
doc_env.set('DOXYGEN_ENABLED_SECTIONS', ' '.join(get_option('enabled_doxygen_sections')))
doc_defines = [ 'VK_VERSION_1_0=1', '__DOXYGEN__=1' ]
doc_env.set('DOXYGEN_PREDEFINED', ' '.join(doc_defines))
docs = custom_target(output: 'documentation', build_always_stale: true,
                     command: [ doxygen, files('Doxyfile') ], env: doc_env, depends: headers)
