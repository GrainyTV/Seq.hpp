project('seq.hpp', 'cpp',
    default_options: {
        'cpp_std': 'c++17',
        'warning_level': '3',
        'buildtype': 'release',
    }
)

if get_option('tests') == true
    catch2 = dependency('catch2-with-main')

    catch2_defines = [
        '-DCATCH_CONFIG_CONSOLE_WIDTH=50',
        '-DCATCH_CONFIG_ENABLE_PAIR_STRINGMAKER',
    ]

    test_exe = executable('testing', 'test/main.cpp',
        include_directories: 'hdr',
        dependencies: catch2,
        cpp_args: catch2_defines,
    )

    test('Run Catch2 Test Cases', test_exe)

endif

if get_option('build_config') == 'header-only'
    seq_hpp_dep = declare_dependency(include_directories: 'hdr')
    meson.override_dependency(meson.project_name(), seq_hpp_dep)

else
    error('Not yet implemented!')

endif
