# @brief Prune the PATH environment variable to only include necessary paths.
#        This is necessary because the PATH environment variable on Windows
#        can get very long and cause issues with the CMake generator.
#

set -e  # Exit on error

# Return if not windows
if [[ "$OSTYPE" != "msys" ]]; then return; fi

echo "[windows-prune-PATH.sh] Pruning PATH environment variable..."

CMAKE_DIR=$(dirname $(which cmake))

# [NOTE] You should modify here to include the paths that are necessary for your build.
new_path=$(echo "$PATH" | tr ':' '\n' | \
    awk -v IGNORECASE=1 '
    /(usr\/bin|cuda|windows kits|visual studio)/ {
        if (!seen[$0]++) print
    }' | paste -sd ':' -)

PATH="${CMAKE_DIR}:${new_path}"

echo "[windows-prune-PATH.sh] PATH environment variable pruned."