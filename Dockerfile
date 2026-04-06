# FROM linuxmintd/mint21.1-amd64
FROM phusion/baseimage:noble
# FROM amd64/ubuntu:noble

RUN apt update && DEBIAN_FRONTEND=noninteractive

RUN apt-get install -y build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev

COPY . /sand/
WORKDIR /sand

RUN cmake -B build -S . -DBUILD_TESTING=ON
RUN cmake --build build

CMD /bin/bash
# CMD build/Debug/game_test
