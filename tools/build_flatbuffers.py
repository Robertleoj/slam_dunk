import subprocess
from pathlib import Path
import shutil
import os
import filecmp
import tempfile

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
    Compile all .fbs files in schema_dir using flatc into out_dir,
    only overwriting if output differs.
    """
    build_flatc()
    os.makedirs(OUT_DIR, exist_ok=True)

    for fname in os.listdir(SCHEMA_DIR):
        if fname.endswith(".fbs"):
            schema_file = SCHEMA_DIR / fname
            with tempfile.TemporaryDirectory() as tmpdir:
                tmpdir_path = Path(tmpdir)
                subprocess.run(
                    [
                        FLATBUFFER_EXECUTABLE_PATH,
                        "-o",
                        tmpdir_path,
                        "--cpp",
                        schema_file,
                    ],
                    check=True,
                )

                for gen_file in tmpdir_path.iterdir():
                    out_file = OUT_DIR / gen_file.name
                    if not out_file.exists() or not filecmp.cmp(
                        gen_file, out_file, shallow=False
                    ):
                        shutil.copy2(gen_file, out_file)


if __name__ == "__main__":
    compile_flatbuffers()
