project('necrowarp', 'cpp', version: '0.0.1')

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

    includes = include_directories(['src', '/home/balin/Projects/quasi-msys2/root/ucrt64/include/SDL2', '/home/balin/Projects/sdk/public'])

    std_deps = [
        cxx.find_library('gcc_s', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('stdc++', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('winpthread', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('steam_api64', dirs: '/home/balin/Projects/sdk/redistributable_bin/win64', required: true, static: false),
    ]

    sdl_deps = [
        cxx.find_library('SDL2', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('SDL2_image', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('SDL2_mixer', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('SDL2_net', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
        cxx.find_library('SDL2_ttf', dirs: '/home/balin/Projects/quasi-msys2/root/ucrt64/lib', required: true, static: true),
    ]

    dyn_libs = ['/home/balin/Projects/sdk/redistributable_bin/win64/steam_api64.dll']

elif target_machine.system() == 'linux'

    includes = include_directories(['src', '/usr/include/magic_enum', '/usr/local/include/SDL2'])

    std_deps = [
        cxx.find_library('gcc_eh', required: true, static: true),
        cxx.find_library('stdc++', required: true, static: true),
        cxx.find_library('pthread', required: true, static: true),
        cxx.find_library('steam_api', dirs: '/home/balin/Projects/sdk/redistributable_bin/win64', required: true, static: false),
    ]

    sdl_deps = [
        cxx.find_library('SDL2', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_image', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_mixer', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_net', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_ttf', dirs: '/usr/local/lib', required: true, static: true),
    ]

    dyn_libs = ['/home/balin/Projects/sdk/redistributable_bin/linux64/libsteam_api.so']

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
