project(
    'necrowarp',
    'cpp',
    version: '0.0.1',
)

install_dir = '@0@/install/@1@/@2@'.format(
	meson.project_source_root(),
	target_machine.system(),
	target_machine.cpu_family()
)

build_type = get_option('buildtype')

if build_type == 'debug'
    subsystem = 'console'
    args = ['-DBLEAK_DEBUG']

	install_data('steam_appid.txt', install_dir: install_dir)
else
    subsystem = 'windows'
    args = ['-DBLEAK_RELEASE', '-DNDEBUG']
endif

cxx = meson.get_compiler('cpp')

if target_machine.system() == 'windows'

    includes = include_directories(['src', 'c:/dev/msys64/ucrt64/include', 'subprojects/libbleak', 'c:/dev/steam/sdk/public'])

    std_deps = [
        cxx.find_library('gcc', required: true, static: true),
        cxx.find_library('gcc_s', required: true, static: true),
        cxx.find_library('stdc++', required: true, static: true),
        cxx.find_library('winpthread', required: true, static: true),
        cxx.find_library('steam_api', dirs: 'c:/dev/steam/sdk/redistributable_bin/win64', required: true, static: false),
    ]

    sdl_deps = [
        dependency('SDL2', required: true, static: true),
        dependency('SDL2_image', required: true, static: true),
        dependency('SDL2_mixer', required: true, static: true),
        dependency('SDL2_net', required: true, static: true),
        dependency('SDL2_ttf', required: true, static: true),
    ]

    dyn_libs = []

elif target_machine.system() == 'linux'

    includes = include_directories(['src', '/usr/local/include/SDL2', 'subprojects/libbleak'])

    std_deps = [
        cxx.find_library('gcc', required: true, static: true),
        cxx.find_library('gcc_s', required: true, static: false),
        cxx.find_library('stdc++', required: true, static: true),
        cxx.find_library('pthread', required: true, static: true),
        cxx.find_library('steam_api', required: true, static: false),
    ]

    sdl_deps = [
        cxx.find_library('SDL2', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_image', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_mixer', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_net', dirs: '/usr/local/lib', required: true, static: true),
        cxx.find_library('SDL2_ttf', dirs: '/usr/local/lib', required: true, static: true),
    ]

    dyn_libs = [
        '/usr/lib/libgcc_s.so.1',
        '/usr/lib/libstdc++.so.6',
        '/usr/lib/libsteam_api.so',
        '/usr/lib/libSDL2-2.0.so.0',
        '/usr/lib/libSDL2_image-2.0.so.0',
        '/usr/lib/libSDL2_mixer-2.0.so.0',
        '/usr/lib/libSDL2_net-2.0.so.0',
        '/usr/lib/libSDL2_ttf-2.0.so.0',
        '/usr/lib/libm.so.6',
        '/usr/lib/libc.so.6',
        '/usr/lib64/ld-linux-x86-64.so.2',
        '/usr/lib/libdl.so.2',
        '/usr/lib/libpthread.so.0',
        '/usr/lib/libpng16.so.16',
        '/usr/lib/libjxl.so.0.11',
        '/usr/lib/libjpeg.so.8',
        '/usr/lib/libtiff.so.6',
        '/usr/lib/libavif.so.16',
        '/usr/lib/libwebpdemux.so.2',
        '/usr/lib/libwebp.so.7',
        '/usr/lib/libfreetype.so.6',
        '/usr/lib/libharfbuzz.so.0',
        '/usr/lib/libz.so.1',
        '/usr/lib/libjxl_cms.so.0.11',
        '/usr/lib/libhwy.so.1',
        '/usr/lib/libbrotlidec.so.1',
        '/usr/lib/libbrotlienc.so.1',
        '/usr/lib/libzstd.so.1',
        '/usr/lib/liblzma.so.5',
        '/usr/lib/libjbig.so.2.1',
        '/usr/lib/libyuv.so',
        '/usr/lib/libsharpyuv.so.0',
        '/usr/lib/libdav1d.so.7',
        '/usr/lib/librav1e.so.0.7',
        '/usr/lib/libSvtAv1Enc.so.3',
        '/usr/lib/libaom.so.3',
        '/usr/lib/libbz2.so.1.0',
        '/usr/lib/libglib-2.0.so.0',
        '/usr/lib/libgraphite2.so.3',
        '/usr/lib/libbrotlicommon.so.1',
        '/usr/lib/libpcre2-8.so.0',
    ]

endif

libbleak = subproject('libbleak', required: true)

libbleak_both = libbleak.get_variable('libbleak')

executable(
    'necrowarp',

    'src/program.cpp',

    dependencies: [std_deps, sdl_deps],

    link_whole: [libbleak_both.get_static_lib()],

    include_directories: includes,

    c_args: args,
    cpp_args: args,

    install: true,
    install_dir: install_dir,

    win_subsystem: subsystem,
)

install_subdir('log', install_dir: install_dir)
install_subdir('res', install_dir: install_dir)

install_data('log/message.log', install_dir: install_dir + '/log')
install_data('log/error.log', install_dir: install_dir + '/log')

install_data(dyn_libs, install_dir: install_dir, follow_symlinks: true)
