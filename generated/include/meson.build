headers += custom_target('command_tags.inl', output: '@BASENAME@', input: files('command_tags.inl.in'),
                         command: generator + template_arguments + [ '@INPUT@' ], install: true,
                         install_dir: 'include/megatech/vulkan')
