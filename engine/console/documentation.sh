#!/bin/bash

doxygen Doxyfile

# clear and move doxygen generated files to root directory in documentation
pushd ../../
  if [ ! -d "documentation" ]; then
    mkdir documentation
  fi

  rm -r documentation/html
  rm -r documentation/latex

  mv engine/main/html documentation
  mv engine/main/latex documentation

# shellcheck disable=SC2164
popd
