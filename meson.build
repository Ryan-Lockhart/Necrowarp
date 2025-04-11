project('necrowarp', 'cpp', version: '0.0.1', meson_version: '>=1.3.0')

install_dir = '@0@/install/@1@/@2@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family()
)

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'

	install_data('steam_appid.txt', install_dir: install_dir)
else
    subsystem = 'windows'
endif

cxx = meson.get_compiler('cpp')

if target_machine.system() == 'windows'

    includes = include_directories(['src', 'C:/dev/msys64/ucrt64/include/SDL2', 'C:/dev/steam/include', 'C:/dev/external/magic_enum/include'])

    std_deps = [
        cxx.find_library('gcc_s', dirs: 'C:/dev/msys64/ucrt64/bin', required: true, static: true),
        cxx.find_library('stdc++', dirs: 'C:/dev/msys64/ucrt64/bin', required: true, static: true),
        cxx.find_library('winpthread', dirs: 'C:/dev/msys64/ucrt64/bin', required: true, static: true),
        cxx.find_library('steam_api64', dirs: 'C:/dev/steam/bin/win64', required: true, static: false),
    ]

    sdl_deps = [
        dependency('SDL2', required: true, static: true),
        dependency('SDL2_image', required: true, static: true),
        dependency('SDL2_mixer', required: true, static: true),
    ]

    dyn_libs = ['C:/dev/steam/bin/win64/steam_api64.dll']

elif target_machine.system() == 'linux'

    root_dir = 'L:/'

    includes = include_directories(['src', root_dir + 'usr/include/SDL2', 'C:/dev/steam/include', 'C:/dev/external/magic_enum/include'])

    std_deps = [
        cxx.find_library('gcc_eh', dirs: root_dir + 'usr/lib/gcc/x86_64-pc-linux-gnu/14.2.1', required: true, static: true),
        cxx.find_library('stdc++', dirs: root_dir + 'usr/lib', required: true, static: true),
        cxx.find_library('pthread', dirs: root_dir + 'usr/lib', required: true, static: true),
        cxx.find_library('steam_api', dirs: 'C:/dev/steam/bin/linux64', required: true, static: false),
    ]

    sdl_deps = [
        cxx.find_library('SDL2', dirs: root_dir + 'usr/lib', required: true, static: true),
        cxx.find_library('SDL2_image', dirs: root_dir + 'usr/lib', required: true, static: true),
        cxx.find_library('SDL2_mixer', dirs: root_dir + 'usr/lib', required: true, static: true),
    ]

    dyn_libs = ['C:/dev/steam/bin/linux64/libsteam_api.so']

endif

bleak = subproject('libbleak', required: true)

bleak_include = bleak.get_variable('libbleak_include')
bleak_args = bleak.get_variable('libbleak_args')

executable(
    'necrowarp',

    'src/program.cpp',

    dependencies: [std_deps, sdl_deps],

    include_directories: [includes, bleak_include],

    c_args: bleak_args,
    cpp_args: bleak_args,

    install: true,
    install_dir: install_dir,

    win_subsystem: subsystem,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)

install_data('log/message.log', install_dir: install_dir + '/log')
install_data('log/error.log', install_dir: install_dir + '/log')

install_data(dyn_libs, install_dir: install_dir, follow_symlinks: true)
