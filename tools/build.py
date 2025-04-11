#!/usr/bin/env python3

"""A script to build the C++ backend and install the bindings into the source tree."""

import shutil
import os
import subprocess
from functools import partial
from pathlib import Path

from fire import Fire

BUILD_DIR = "build"


def build(debug: bool) -> None:
    """(Re)build the C++ backend."""

    build_path = Path("build")
    build_path.mkdir(exist_ok=True)

    compile_cmd = [
        "cmake",
        "-B",
        str(build_path),
        "-G",
        "Ninja",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=1",
        "-DCMAKE_C_COMPILER=/usr/bin/gcc-13",
        "-DCMAKE_CXX_COMPILER=/usr/bin/g++-13",
        '-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_MODULE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld"',
    ]

    if debug:
        compile_cmd.append("-DCMAKE_BUILD_TYPE=Debug")

    subprocess.run(
        compile_cmd,
        check=True,
    )

    subprocess.run(["ninja", "-C", str(build_path)])

    subprocess.run(
        [
            "pybind11-stubgen",
            "slamd.bindings",
            "--numpy-array-remove-parameters",
            "-o",
            "python_bindings/src",
        ],
        env={**os.environ, "PYTHONPATH": "./build/python_bindings"},
        check=True,
    )


def clean() -> None:
    """Clean the build folder and remove the symlink, if any."""
    shutil.rmtree(BUILD_DIR, ignore_errors=True)


def clean_build() -> None:
    """First clean and then build."""
    clean()
    build(False)


def debug_build():
    build(True)


def clean_debug_build():
    clean()
    build(True)


def check_in_repo() -> None:
    """Check that we are executing this from repo root."""
    assert Path(".git").exists(), "This command should run in repo root."


if __name__ == "__main__":
    check_in_repo()
    # os.chdir(Path("./examples"))

    Fire(
        {
            "build": partial(build, False),
            "clean": clean,
            "clean_build": clean_build,
            "debug_build": debug_build,
            "clean_debug_build": clean_debug_build,
        }
    )
