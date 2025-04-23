import subprocess
from pathlib import Path
import shutil
import os

REPO_DIR = Path(__file__).parent.parent

FLATBUFFERS_DIR = (REPO_DIR / "vendored_deps/flatbuffers").resolve()
BUILD_DIR = FLATBUFFERS_DIR / "build"

FLATBUFFER_EXECUTABLE_PATH = BUILD_DIR / "flatc"

SCHEMA_DIR = REPO_DIR / "flatbuffers"

OUT_DIR = REPO_DIR / "slamd/flatb/flatb"


def build_flatc(force: bool = False):
    if FLATBUFFER_EXECUTABLE_PATH.exists() and not force:
        return

    shutil.rmtree(BUILD_DIR, ignore_errors=True)

    subprocess.run(
        [
            "cmake",
            "-B",
            BUILD_DIR,
            "-S",
            FLATBUFFERS_DIR,
            "-G",
            "Ninja",
            "-DFLATBUFFERS_BUILD_TESTS=OFF",
        ],
        check=True,
    )
    subprocess.run(["ninja", "-C", BUILD_DIR, "flatc"], check=True)


def compile_flatbuffers():
    """
    Compile all .fbs files in schema_dir using flatc into out_dir
    """
    build_flatc()
    os.makedirs(OUT_DIR, exist_ok=True)

    for fname in os.listdir(SCHEMA_DIR):
        if fname.endswith(".fbs"):
            schema_file = os.path.join(SCHEMA_DIR, fname)
            subprocess.run(
                [
                    FLATBUFFER_EXECUTABLE_PATH,
                    "-o",
                    OUT_DIR,
                    "--cpp",  # change this if you want other output (e.g., --cpp, --go, etc.)
                    schema_file,
                ],
                check=True,
            )


if __name__ == "__main__":
    compile_flatbuffers()
