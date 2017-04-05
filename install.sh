#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root due to it installing packages as requirements"
   echo "Either rerun this scripts as root, or install prerequisite packages and manually compile"
   exit 1
fi

OS=-1

if [[ -e "/etc/arch-release" ]]; then
    echo "Arch distro"
    OS=1
elif [[ -e "/etc/fedora-release" ]]; then
    echo "Fedora distro"
    OS=2
elif [[ -e "/etc/lsb-release" ]]; then
    echo "Debian/Ubuntu distro"
    OS=3
else
    echo "Unknown distro"
    exit 1
fi

if [[ $OS -eq 1 ]]; then
    pacman -S sdl2 sdl2_image sdl2_mixer sdl2_ttf smpeg2 --noconfirm
elif [[ $OS -eq 2 ]]; then
    dnf install -y SDL2*
else
    apt-get install -y SDL2*
fi

make clean
make -j$(nproc --all) all

if [[ -n "$SUDO_COMMAND" ]]; then
    chown $SUDO_UID -R $PWD/bin
fi

echo "The game has been successfully installed and compiled"
echo "To play, just run './bin/Linux_Game'"
echo "To run the server, just run './bin/server'"
echo "Have fun!"
