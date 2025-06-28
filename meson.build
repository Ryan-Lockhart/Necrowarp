project('necrowarp', 'cpp', version: '0.1.1', meson_version: '>=1.3.0')

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'
    necrowarp_args = ['-DBLEAK_DEBUG']
else
    subsystem = 'windows'
    necrowarp_args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

install_dir = '@0@/install/@1@/@2@/@3@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family(),
    build_type
)

if build_type == 'debug'
	install_data('steam_appid.txt', install_dir: install_dir)
endif

cxx = meson.get_compiler('cpp')

if target_machine.system() == 'windows'

    necrowarp_private_includes = include_directories('src', 'C:/dev/msys64/ucrt64/include/SDL2', 'C:/dev/steam/include', 'C:/dev/external/magic_enum/include')

    dyn_libs = ['C:/dev/steam/bin/win64/steam_api64.dll']

elif target_machine.system() == 'linux'

    necrowarp_private_includes = include_directories('src', '/usr/local/include/SDL2', '/usr/local/include')

    dyn_libs = ['/usr/local/lib/libsteam_api.so']

endif

subproject('bleak')

bleak_dep = dependency('bleak', required: true, fallback: ['bleak', 'bleak_dep'])

subdir('inc')

executable('necrowarp', 'src/program.cpp',
	cpp_args: ['-march=x86-64-v3', necrowarp_args],

    include_directories: [necrowarp_private_includes, necrowarp_internal_includes],
    dependencies: bleak_dep,

    install: true,
    install_dir: install_dir,

    win_subsystem: subsystem,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)

install_data('log/message.log', install_dir: install_dir + '/log')
install_data('log/error.log', install_dir: install_dir + '/log')

install_data(dyn_libs, install_dir: install_dir, follow_symlinks: true)
