#!/bin/bash
current_dir=$( dirname -- "${BASH_SOURCE[0]}"; )
current_dir="$( realpath -e -- "$current_dir"; )"
pushd $current_dir/../..
sources_app_dir="$(pwd)/sources/app"
sources_external_dir="$(pwd)/sources/external"
cpp_files=$(find ./sources -name "*.cpp" -exec realpath {} \;)
pushd bin
clang++ $cpp_files -o pingpong -I $sources_app_dir -I $sources_external_dir -lX11 -lGL -GLX 
popd
popd
