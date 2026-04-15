project(
    'Seq.hpp',
    'cpp',
    version: '0.0.1',
    meson_version: '>=1.2.0',
    default_options: {
        'cpp_std': 'c++20',
    },
)

# ┏━━━━━━━━━┓
# ┃ Options ┃
# ┗━━━━━━━━━┛
enable_dev = get_option('enable_dev')

# ┏━━━━━━━━━┓
# ┃ Defines ┃
# ┗━━━━━━━━━┛
project_name = meson.project_name()
project_dir = meson.current_source_dir()
build_dir = meson.current_build_dir()
header_dir = 'hdr'

test_dir = 'test'
test_out_dir = build_dir / test_dir

examples_dir = 'examples'
examples_out_dir = build_dir / examples_dir

script_dir = project_dir / 'scripts'

# ┏━━━━━━━━━━━━━━━━━━━┓
# ┃ Script Entrypoint ┃
# ┗━━━━━━━━━━━━━━━━━━━┛
seq_hpp_dep = declare_dependency(
    include_directories: header_dir,
)
meson.override_dependency(project_name, seq_hpp_dep)

if enable_dev
    target_name = 'dev'
    example_ids = ['001_readme', '005_ienumerable']
    examples = []

    foreach id : example_ids
        examples += executable(
            id,
            examples_dir / id / '@0@.cpp'.format(id),
            dependencies: seq_hpp_dep,
            install: true,
            install_dir: examples_out_dir,
        )

    endforeach

    cpp_tests = executable(
        'cpp_tests',
        test_dir / 'main.cpp',
        dependencies: seq_hpp_dep,
        include_directories: test_dir,
        install: true,
        install_dir: test_out_dir,
    )

    run_target(
        target_name,
        command: [script_dir / '@0@.py'.format(target_name)],
        depends: [examples, cpp_tests],
        env: {
            'MESON': find_program('meson').full_path(),
            'NINJA': find_program('ninja').full_path(),
            'EXAMPLES_DIR': examples_out_dir,
            'CPP_TEST_SUITE': cpp_tests.full_path(),
        },
    )

endif
