#!/usr/bin/env bash

##############################################################################
# Functions
function usage {
  echo "usage: ${BASH_SOURCE[0]} <target architecture> [host os] [source dir]" >&2
  echo "  If no source directory is given, the current directory is assumed." >&2 
}

##############################################################################
# Parameter checking
ARCH="${1}"
test -z "${ARCH}" && (usage ; exit 1)
HOST_OS="${2}"
test -z "${HOST_OS}" && HOST_OS="$(uname -o)"
SOURCE_DIR="${3}"
test -z "${SOURCE_DIR}" && SOURCE_DIR="$(pwd)"
CLEAN_RUN="${5}"
test -z "${CLEAN_RUN}" && CLEAN_RUN=false

##############################################################################
# Constants and defaults

# Directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

##############################################################################
# Load functions
. "${SCRIPT_DIR}/qemu-chroot-build.sh"

##############################################################################
# Main

set -e
set -x

# First, try to setup & enter chroot if necessary.
ret=0
chroot_try_enter "${BASH_SOURCE[0]}" "${ARCH}" "${HOST_OS}" "${SOURCE_DIR}" "focal" "" "${CLEAN_RUN}" "$@" || ret="$?"

case "${ret}" in
  1)
    exit "$ret"
    ;;
  2)
    chroot_clean
    exit "$ret"
    ;;
  3)
    chroot_clean
    exit 0
    ;;
esac

cd "${SOURCE_DIR}"
cd ..

echo "NOTE: use 'exit' command to leave chrootted shell!"

#Run interactive shell
bash

# By default only unmount dev and proc
chroot_clean