project('necrowarp', 'cpp', version: '0.0.1', meson_version: '>=1.3.0')

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'
else
    subsystem = 'windows'
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
    includes = include_directories(['src', 'C:/dev/msys64/ucrt64/include/SDL2', 'C:/dev/steam/include', 'C:/dev/external/magic_enum/include'])

    std_deps = [
        cxx.find_library('gcc_eh', dirs: 'C:/dev/msys64/ucrt64/lib/gcc/x86_64-w64-mingw32/15.1.0', required: true, static: true),
        cxx.find_library('stdc++', dirs: 'C:/dev/msys64/ucrt64/lib', required: true, static: true),
        cxx.find_library('winpthread', dirs: 'C:/dev/msys64/ucrt64/lib', required: true, static: true),
        cxx.find_library('steam_api64', dirs: 'C:/dev/steam/bin/win64', required: true, static: false),
    ]

    dyn_libs = ['C:/dev/steam/bin/win64/steam_api64.dll']
elif target_machine.system() == 'linux'
    includes = include_directories(['src', '/usr/local/include/SDL2', '/usr/local/include'])

    std_deps = [
        cxx.find_library('gcc_eh', dirs: '/usr/lib/gcc/x86_64-pc-linux-gnu/15.1.1', required: true, static: true),
        cxx.find_library('stdc++', dirs: '/usr/lib', required: true, static: true),
        cxx.find_library('pthread', dirs: '/usr/lib', required: true, static: true),
        cxx.find_library('steam_api', dirs: '/usr/local/lib', required: true, static: false),
    ]

    dyn_libs = ['/usr/local/lib/libsteam_api.so']
endif

sdl_deps = [
    dependency('SDL2', required: true, static: true),
    dependency('SDL2_image', required: true, static: true),
    dependency('SDL2_mixer', required: true, static: true),
    dependency('SDL2_net', required: true, static: true),
]

bleak_dep = dependency('bleak', required: true, fallback: ['libbleak', 'bleak_dep'])

executable('necrowarp', 'src/program.cpp',
	cpp_args: ['-DSTEAMLESS'],

    include_directories: [includes],
    dependencies: [std_deps, sdl_deps, bleak_dep],

    install: true,
    install_dir: install_dir,

    win_subsystem: subsystem,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)

install_data('log/message.log', install_dir: install_dir + '/log')
install_data('log/error.log', install_dir: install_dir + '/log')

install_data(dyn_libs, install_dir: install_dir, follow_symlinks: true)
