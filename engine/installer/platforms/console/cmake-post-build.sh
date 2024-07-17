#!/bin/bash

pushd ../../../../builds/console || exit
  if [ ! -d 'libs' ]; then
    echo "Linking libraries"
    # shellcheck disable=SC2226
    ln -s ../../libs
  fi

  if [ !  -d 'assets' ]; then
    echo "Linking Assets Data"
    # shellcheck disable=SC2226
    ln -s ../../engine/assets
  fi

  if [ !  -d 'third-party' ]; then
    echo "Linking Tracy Server Profiler"
    # shellcheck disable=SC2226
    ln -s ../../third-party/tracy/profiler
  fi
popd || exit

echo "Adding Frameworks @rpath to binary ..."
## TODO: Some error on this file

install_name_tool -add_rpath @loader_path/libs/Frameworks ../../../../builds/console/Physicat