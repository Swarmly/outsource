#!/usr/bin/env bash
_BUILD_TYPE="$1"
_PROJECT_DIR="$2"
_PACKAGE_TARGET_NAME="$3"

export PATH="$MINGW_PREFIX/bin:$PATH"
export PKG_CONFIG_PATH="$MINGW_PREFIX/lib/pkgconfig:$PKG_CONFIG_PATH"
cd $_PROJECT_DIR

_PROJECT_DIR_NAME=${PWD##*/}

_LOWERED_BUILD_TYPE=${_BUILD_TYPE,,}
_CMAKE_BUILD_DIR_NAME="cmake-build-msys-$_LOWERED_BUILD_TYPE"
mkdir -p "../$_CMAKE_BUILD_DIR_NAME" && cd "../$_CMAKE_BUILD_DIR_NAME"

cmake "../$_PROJECT_DIR_NAME" -GNinja -DCMAKE_BUILD_TYPE=$_BUILD_TYPE
ninja -j7

# packaging if enabled
if [ -n "$_PACKAGE_TARGET_NAME" ]; then
  echo ""
  echo ""
  echo "Packaging..."
  ninja "$_PACKAGE_TARGET_NAME"
fi