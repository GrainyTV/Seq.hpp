project('seq.hpp', 'cpp',
    default_options: {
        'cpp_std': 'c++17',
        'warning_level': '3'
    }
)

catch2 = dependency('catch2-with-main')

executable('testing', 'test/main.cpp',
    include_directories: 'hdr',
    dependencies: catch2
)
