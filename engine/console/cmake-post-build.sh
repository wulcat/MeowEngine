#!/bin/bash

echo "Adding Frameworks @rpath to binary ..."
# TODO: Some error on this file
install_name_tool -add_rpath @loader_path/../Frameworks out/console

pushd out
  if [ !  -d 'assets' ]; then
    echo "Linking Assets Data"
    ln -s ../../main/assets
  fi
popd

pushd out
  if [ !  -d 'third-party' ]; then
    echo "Linking Tracy Server Profiler"
    ln -s ../../../third-party/tracy/profiler
  fi
popd