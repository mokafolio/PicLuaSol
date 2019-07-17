project('PicLuaSol', 'cpp', default_options : ['cpp_std=c++14'])

#remove this rather bogus warning
add_project_arguments('-Wno-missing-braces', language: 'cpp')

incDirs = include_directories('.')

if meson.is_subproject() == false or get_option('forceInstallHeaders')
    install_headers('PicLuaSol/PicLuaSol.hpp', subdir: 'PicLuaSol')
endif

picLuaDep = declare_dependency(include_directories: incDirs)
