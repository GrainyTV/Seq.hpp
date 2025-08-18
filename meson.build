project('seq.hpp', 'cpp',
    default_options: {
        'cpp_std': 'c++17',
        'warning_level': '3',
        'buildtype': 'release',
    }
)

if get_option('tests') == true
    catch2 = dependency('catch2-with-main')

    test_exe = executable('catch2_tests', 'test/main.cpp',
        include_directories: 'hdr',
        dependencies: catch2,
    )

    test('Run Catch2 Test Cases', test_exe,
        args: ['--rng-seed', '42', '--reporter', 'TAP'],
        protocol: 'tap',
        verbose: true,
    )

endif

if get_option('build_config') == 'header-only'
    seq_hpp_dep = declare_dependency(include_directories: 'hdr')
    meson.override_dependency(meson.project_name(), seq_hpp_dep)

else
    error('Static library build is not yet supported!')

endif
