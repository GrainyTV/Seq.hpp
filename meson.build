project('seq.hpp', 'cpp',
    default_options: {
        'cpp_std': 'c++17',
        'warning_level': '3'
    }
)

catch2 = dependency('catch2-with-main')

catch2_defines = [
    '-DCATCH_CONFIG_CONSOLE_WIDTH=50',
    '-DCATCH_CONFIG_ENABLE_PAIR_STRINGMAKER',
]

executable('testing', 'test/main.cpp',
    include_directories: 'hdr',
    dependencies: catch2,
    cpp_args: catch2_defines,
)
