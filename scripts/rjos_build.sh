#!/bin/bash

set -e

build_project() {
  BUILD_TYPE=$1
  BUILD_DIR="bin/$BUILD_TYPE"
  BUILD_TARGET=$2

  echo "Building RJOS example: $BUILD_TARGET with configuration $BUILD_TYPE"

  # Create the build directory and configure the project:
  cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -B "$BUILD_DIR"

  # Build the RJOS example:
  cmake --build "$BUILD_DIR" --target "$BUILD_TARGET"

  # Check if the build was successful:
  if [ $? -eq 0 ]; then
    echo "Build completed successfully: $BUILD_TYPE"
  else
    echo "build failed: $BUILD_TYPE"
    exit 1
  fi
}

# Ensure required tools are installed
if ! command -v cmake &>/dev/null; then
  echo "Error: cmake is not installed or not in PATH."
  exit 1
fi

if ! command -v ninja &>/dev/null; then
  echo "Error: ninja is not installed or not in PATH."
  exit 1
fi
