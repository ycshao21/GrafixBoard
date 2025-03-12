# Env Variables: CC, CXX, NVCC_CCBIN

set -e  # Exit on error

SOURCE_DIR=.
BUILD_DIR=./build
BUILD_TYPE=Release
CXX_STANDARD=20
CUDA_STANDARD=20
CMAKE_TOOL_CHAIN_FILE=""

# Check if stdout is terminal -> enable/disable colored output
if [ -t 1 ]; then 
    STDOUT_IS_TERMINAL=ON; export GTEST_COLOR=yes
else
    STDOUT_IS_TERMINAL=OFF; export GTEST_COLOR=no
fi

# Check and set CMAKE_TOOL_CHAIN_FILE to vcpkg.cmake
if [ -f "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" ]; then
    CMAKE_TOOL_CHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
elif [ -f "$VCPKG_HOME/scripts/buildsystems/vcpkg.cmake" ]; then
    CMAKE_TOOL_CHAIN_FILE="$VCPKG_HOME/scripts/buildsystems/vcpkg.cmake"
else
    echo "[build.sh][ERROR] ENV:VCPKG_ROOT or ENV:VCPKG_HOME is not set or " \
        "vcpkg.cmake is not found. Please install vcpkg and set VCPKG_ROOT " \
        "or VCPKG_HOME to the vcpkg root directory"
    exit 1
fi

while [[ $# -gt 0 ]]; do
    case $1 in
        -S|--source-dir)
            SOURCE_DIR=$2; shift ;;
        -B|--build-dir)
            BUILD_DIR=$2; shift ;;
        Release|Debug)
            BUILD_TYPE=$1 ;;
        --stdc++=*)
            CXX_STANDARD="${1#*=}" ;;
        --stdcuda=*)
            CUDA_STANDARD="${1#*=}" ;;
        --prune-env-path)
            # Takes effects only on windows
            source ./scripts/windows-prune-PATH.sh ;;
        --rm-build-dir)
            rm -rf $BUILD_DIR ;;
        *)
            # [TODO] Add detailed help message
            echo "Unknown argument: $1"; exit 1 ;;
    esac
    shift
done

cmake -G Ninja -S $SOURCE_DIR -B $BUILD_DIR \
    -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOL_CHAIN_FILE \
    -DSTDOUT_IS_TERMINAL=$STDOUT_IS_TERMINAL \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DCMAKE_CXX_STANDARD=$CXX_STANDARD \
    -DCMAKE_CUDA_STANDARD=$CUDA_STANDARD

cmake --build $BUILD_DIR -j $(nproc)