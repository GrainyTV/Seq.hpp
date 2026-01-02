project(
    'Seq.hpp',
    'cpp',
    version: '0.0.1',
    meson_version: '>=1.2.0',
    default_options: {
        'cpp_std': 'c++20',
        'warning_level': '3',
    },
)

project_name = meson.project_name()
header_dir = 'hdr'
test_dir = 'test'

if get_option('tests')
    cpp = meson.get_compiler('cpp')
    prevent_unregistered_test_cases_flag = cpp.get_supported_arguments(
        '-Werror=unused-function',
    )

    test_exe = executable(
        f'@project_name@.tests',
        test_dir / 'main.cpp',
        cpp_args: prevent_unregistered_test_cases_flag,
        include_directories: [header_dir, test_dir],
        override_options: {
            'werror': true,
        },
    )

    test(
        f'@project_name@ Test Cases',
        test_exe,
        protocol: 'exitcode',
    )

endif

seq_hpp_dep = declare_dependency(
    include_directories: header_dir,
    variables: {
        'pch': header_dir / 'seq' / 'seq.hpp',
    },
)

meson.override_dependency(project_name, seq_hpp_dep)
