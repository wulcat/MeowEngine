#!/bin/bash
#../../../
pushd ../../../builds/console || exit

  if [ ! -d 'dependencies' ]; then
    mkdir dependencies
  fi

  if [ ! -d 'dependencies/Framework' ]; then
    pushd dependencies  || exit
      echo "Linking libraries"
      # shellcheck disable=SC2226
      ln -s ../../../libs/Frameworks
    popd || exit
  fi

  if [ !  -d 'assets' ]; then
    echo "Linking Assets Data"
    # shellcheck disable=SC2226
    ln -s ../../engine/assets
  fi

  if [ !  -d 'dependencies/profiler' ]; then
    pushd dependencies || exit
      echo "Linking Tracy Server Profiler"
      # shellcheck disable=SC2226
      ln -s ../../../libs/third-party/tracy/profiler
    popd || exit
  fi
popd || exit

echo "Adding Frameworks @rpath to binary ..."
## TODO: Some error on this file

install_name_tool -add_rpath @loader_path/dependencies/Frameworks ../../../builds/console/MeowEngine