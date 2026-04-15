#!/usr/bin/env python

import os
import subprocess
import sys

MESON_EXE    = os.environ['MESON']
NINJA_EXE    = os.environ['NINJA']
BUILD_DIR    = os.environ['MESON_BUILD_ROOT']
PROJECT_DIR  = os.environ['MESON_SOURCE_ROOT']
EXAMPLES_DIR = os.environ['EXAMPLES_DIR']
TEST_SUITE   = os.environ['CPP_TEST_SUITE']

def printDescription(text: str):
    print(f'\033[1m==> {text}\033[0m')

def runExternalCommand(*tokens: str, cwd: str):
    assert len(tokens) > 0, 'No command was provided for this task'

    try:
        subprocess.run(list(tokens), check=True, cwd=cwd)
    except subprocess.CalledProcessError:
        sys.exit(1)

def invokeExternal(*tokens: str, description: str, cwd: str):
    printDescription(description)
    runExternalCommand(*tokens, cwd=cwd)

def invokeExecutablesFromDir(description: str, cwd: str):
    printDescription(description)

    for exe in sorted(os.listdir(cwd)):
        print(f'[{exe}]')
        runExternalCommand(os.path.join(cwd, exe), cwd=cwd)

def main():
    invokeExternal(
        MESON_EXE, 'install',
        description='Installing script dependencies',
        cwd=BUILD_DIR
    )

    invokeExternal(
        TEST_SUITE,
        description='Running C++ test suite',
        cwd=BUILD_DIR
    )

    invokeExecutablesFromDir(
        description='Running examples',
        cwd=EXAMPLES_DIR
    )

    invokeExternal(
        NINJA_EXE, 'clang-format',
        description='Formatting C++ project files',
        cwd=BUILD_DIR
    )

    invokeExternal(
        MESON_EXE, 'format', '--inplace',
        description='Formatting meson.build file',
        cwd=PROJECT_DIR
    )

if __name__ == '__main__':
    main()
