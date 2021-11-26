#!/usr/bin/env bash
# -------------------------------------------------------------------------------
# In theory this script installs all dependencies required for building Hermes with MSYS2
# Execute it once on an MSYS shell.
#
# MSYS2 and installed libraries can be updated later by executing
#   pacman -Syu
# in an MSYS shell
# -------------------------------------------------------------------------------

echo "Selected ARCH: ${MINGW_PACKAGE_PREFIX}"

#echo "Sync pacman package databases"
#pacman -Sy

echo "Installing development tools"
# install basic development system, compiler toolchain and build tools
pacman -S --needed --noconfirm --color=auto \
mc \
msys2-devel \
make \
tar \
git \
p7zip \
$MINGW_PACKAGE_PREFIX-toolchain \
$MINGW_PACKAGE_PREFIX-make \
$MINGW_PACKAGE_PREFIX-pkg-config \
$MINGW_PACKAGE_PREFIX-SDL2  \
$MINGW_PACKAGE_PREFIX-boost \
$MINGW_PACKAGE_PREFIX-ffmpeg \
$MINGW_PACKAGE_PREFIX-toolchain \
$MINGW_PACKAGE_PREFIX-cmake \
$MINGW_PACKAGE_PREFIX-libass \
$MINGW_PACKAGE_PREFIX-ffmpeg \
$MINGW_PACKAGE_PREFIX-ninja \
$MINGW_PACKAGE_PREFIX-qt-creator \
$MINGW_PACKAGE_PREFIX-qt5 \
$MINGW_PACKAGE_PREFIX-nsis \
$MINGW_PACKAGE_PREFIX-exiv2
# add new dependencies here