#!/bin/bash
current_dir=$( dirname -- "${BASH_SOURCE[0]}"; )
current_dir="$( realpath -e -- "$current_dir"; )"
pushd $current_dir/../..
sources_dir="$(pwd)/sources"
cpp_files=$(find ./sources -name "*.cpp" -exec realpath {} \;)
pushd bin
clang++ $cpp_files -o pingpong -I $sources_dir -lX11 -lGL -GLX 
popd
popd
