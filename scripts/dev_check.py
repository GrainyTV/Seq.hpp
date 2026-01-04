#!/usr/bin/env python3

import os
import subprocess

PROJECT_DIR = os.environ['MESON_SOURCE_ROOT']
BUILD_DIR   = os.environ['MESON_BUILD_ROOT']
MESON_EXE   = os.environ['MESON_EXE']
NINJA_EXE   = os.environ['NINJA_EXE']

class Colors:
    BOLD = "\033[1m"
    END = "\033[0m"

def source_code_formatting():
    source_formatting = subprocess.run([NINJA_EXE, 'clang-format-check'], cwd=BUILD_DIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    success = source_formatting.returncode == 0
    return (success, None if success else "Codebase needs to be reformatted")

def build_script_formatting():
    build_formatting = subprocess.run([MESON_EXE, 'format', '--check-only'], cwd=PROJECT_DIR)
    success = build_formatting.returncode == 0
    return (success, None if success else "Meson build file needs to be reformatted")

def test_cases():
    tests = subprocess.run([MESON_EXE, 'test'], cwd=BUILD_DIR, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    success = tests.returncode == 0
    return (success, None if success else "Not all test cases have passed")

def main():
    passed = True
    checks = [
        source_code_formatting,
        build_script_formatting,
        test_cases,
    ]

    print(Colors.BOLD + '[Meson Development Check]' + Colors.END)

    for check in checks:
        (success, message) = check()

        if not success:
            print(Colors.BOLD + '==> ' + Colors.END + message)
            passed = False

    if passed:
        print(Colors.BOLD + 'All development checks passed' + Colors.END)

if __name__ == '__main__':
    main()
