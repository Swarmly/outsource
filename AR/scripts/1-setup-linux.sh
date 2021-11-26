#!/usr/bin/env bash

echo "Installing development tools"

# install basic development system, compiler toolchain and build tools
sudo apt-get -y install \
qt5-default \
qtdeclarative5-dev \
libqt5serialport5-dev   \
libqt5xmlpatterns5-dev  \
libqt5x11extras5-dev \
libboost-all-dev    \
qtpositioning5-dev \
libavdevice-dev \
libqt5svg5-dev \
libavutil-dev \
libavformat-dev \
libavcodec-dev  \
libavfilter-dev \
libavresample-dev \
libswresample-dev \
build-essential \
pkg-config \
libusb-dev \
libeigen3-dev \
libssl-dev \
libsdl2-dev \
libopencv-dev \
exiv2 \
doxygen \
graphviz \
rsync \
git
# qt5*-dev
# add new dependencies here

# install homebrew
#if ! command -v brew &> /dev/null
#then
#  echo "Installing brew..."
#  curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh | bash -
#  test -d ~/.linuxbrew && eval $(~/.linuxbrew/bin/brew shellenv)
#  test -d /home/linuxbrew/.linuxbrew && eval $(/home/linuxbrew/.linuxbrew/bin/brew shellenv)
#  #test -r ~/.bash_profile && echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.bash_profile
#  #test -r ~/.bashrc && echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.bashrc
#  echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.profile
#fi

#echo "Installing brew dependencies..."
## Install only formulas without transitive dependencies to prevent duplicates between apt-get and brew.
#brew tap KDAB/tap
#brew install --ignore-dependencies kddockwidgets-qt5
