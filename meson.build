project(
    'seq.hpp',
    'cpp',
    version: '0.1',
    default_options: {
        'warning_level': '3',
        'werror': true,
        'buildtype': 'release'
})

cxx = meson.get_compiler('cpp')
hdr = include_directories('hdr')

assert(cxx.compiles('#include "seq/seq.hpp" int main() {}',
    args: ['-std=c++14'],
    include_directories: hdr),
    'Failed to build project with seq.hpp included')

catch2 = subproject('catch2')
catch2_dep = catch2.get_variable('catch2_with_main_dep')

executable(
    '@0@.tests'.format(meson.project_name()),
    'test/test.cpp',
    dependencies: catch2_dep,
    cpp_args: [
        '-std=c++20',
    ],
    include_directories: hdr)
