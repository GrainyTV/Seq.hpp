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
project_dir = meson.current_source_dir()
header_dir = 'hdr'
test_dir = 'test'
example_dir = 'examples'
script_dir = 'scripts'

enable_tests = get_option('tests')
enable_examples = get_option('examples')
dev_build = enable_tests and enable_examples

if enable_tests
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

if enable_examples
    example_ids = ['001_readme']

    foreach id : example_ids
        executable(
            id,
            example_dir / id / f'@id@.cpp',
            include_directories: [header_dir],
        )

    endforeach

endif

if dev_build
    run_target(
        'dev-check',
        command: [script_dir / 'dev_check.py'],
        env: {
            'MESON_EXE': find_program('meson').full_path(),
            'NINJA_EXE': find_program('ninja').full_path(),
        },
    )

endif

seq_hpp_dep = declare_dependency(
    include_directories: header_dir,
    variables: {
        'pch': project_dir / header_dir / 'seq' / 'seq.hpp',
    },
)

meson.override_dependency(project_name, seq_hpp_dep)
