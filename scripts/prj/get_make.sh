#!/bin/bash
current_dir=$( dirname -- "${BASH_SOURCE[0]}"; )
current_dir="$( realpath -e -- "$current_dir"; )"
pushd $current_dir/../../workspaces
if ! [ -d ./unix_makefiles  ]; then
mkdir unix_makefiles
fi
pushd unix_makefiles
cmake -G "Unix Makefiles" ../.. -DPLATFORMS=linux 
popd
popd
