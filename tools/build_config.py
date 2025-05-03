import platform

print(platform.system())

if platform.system() == "Darwin":
    CPP_COMPILER_PATH = "/opt/homebrew/opt/llvm/bin/clang++"
    C_COMPILER_PATH = "/opt/homebrew/opt/llvm/bin/clang"

    CMAKE_FLAGS = [
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=1",
        f"-DCMAKE_C_COMPILER={C_COMPILER_PATH}",
        f"-DCMAKE_CXX_COMPILER={CPP_COMPILER_PATH}",
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.22",
        '-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_MODULE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld"',
    ]
elif platform.system() == "Linux":
    CPP_COMPILER_PATH = "/usr/bin/g++"
    C_COMPILER_PATH = "/usr/bin/gcc"

    CMAKE_FLAGS = [
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=1",
        f"-DCMAKE_C_COMPILER={C_COMPILER_PATH}",
        f"-DCMAKE_CXX_COMPILER={CPP_COMPILER_PATH}",
        "-DCMAKE_POLICY_VERSION_MINIMUM=3.22",
        '-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_MODULE_LINKER_FLAGS="-fuse-ld=lld"',
        '-DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld"',
    ]
