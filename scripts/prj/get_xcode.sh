#!/bin/bash
current_dir=$( dirname -- "${BASH_SOURCE[0]}"; )
current_dir="$( realpath -e -- "$current_dir"; )"
pushd $current_dir/../../workspaces
if ! [ -d ./xcode  ]; then
mkdir unix_makefiles
fi
pushd xcode
cmake -G "Xcode" ../.. -DPLATFORMS=macos
popd
popd
