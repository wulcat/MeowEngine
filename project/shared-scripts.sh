#!/bin/bash

#
fetch_brew_dependency() {
  FORMULA_NAME=$1

  echo "Fetch brew dependency: '$FORMULA_NAME'."

  if brew ls --versions "$FORMULA_NAME" > /dev/null; then
    echo "Dependency '$FORMULA_NAME' is already installed, continuing..."
  else
    echo "Dependency '$FORMULA_NAME' is not installed, installing via homebrew..."
    brew install "$FORMULA_NAME"
  fi
}