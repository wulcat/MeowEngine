#!/bin/bash

#tree -d -o directory_structure.txt
pushd ../main
  tree -d src > directory_structure.txt
  python ../console/create_dot_folder_structure.py

  dot -Tpng directory_structure.dot -o directory_structure.png

  mv directory_structure.png ../../documentation
  rm directory_structure.dot
  rm directory_structure.txt

popd
