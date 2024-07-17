#!/bin/bash

#tree -d -o directory_structure.txt
pushd engine || exit
  tree -d source > directory-tree.txt
  python installer/docs/compile-directory.py

  dot -Tpng directory-tree.dot -o directory-diagram.png

  mv directory-diagram.png ../docs/
  rm directory-tree.dot
  rm directory-tree.txt

popd || exit
