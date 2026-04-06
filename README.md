# Sand
[Wiki](https://github.com/infgotoinf/Sand/wiki/О-проекте)



## Build
### Steps to build on NixOS
1: Clone project's repo
```shell
git clone https://infgotoinf/Sand.git && cd Sand
```
2: Install git modules
```shell
git submodule update --init --recursive
```
3: Enter dev shell
```shell
nix-shell
```
4: Generate config
```shell
cmake -B build
```
5: Build executable
```shell
cmake --build build
```
6: Run it
```shell
build/game
```


### Steps to build on Debian based Linux distribution (Ubuntu, Mint)
1: Install dependences
<!-- https://wiki.libsdl.org/SDL3/README-linux -->
```shell
sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev \
libpipewire-0.3-dev libwayland-dev libdecor-0-dev liburing-dev
```
2: Clone project's repo
```shell
git clone https://infgotoinf/Sand.git && cd Sand
```
3: Install git modules
```shell
git submodule update --init --recursive
```
4: Generate config
```shell
cmake -B build
```
5: Build executable
```shell
cmake --build build
```
6: Run it
```shell
build/game
```


### Steps to build with Docker
1: Clone project's repo
```shell
git clone https://infgotoinf/Sand.git && cd Sand
```
2: Install git modules
```shell
git submodule update --init --recursive
```
3: Install docker and add your user to docker group
```shell
apt install docker docker-buildx
sudo usermod -aG docker USERNAME && newgrp docker
```
4: Install dockerfile
```shell
docker build . -t sand-image
```
5: Run game from the container
> Currently you can't run the game from a Docker container
<!-- ```shell -->
<!-- docker run sand-image:latest build/Debug/game -->
<!-- ``` -->


### Steps to build on Windows
> Alternatevely you can install WSL and follow NixOS, Docker or Linux steps instead.

1: Install MSYS2 https://github.com/msys2/msys2-installer/releases

2: Launch MSYS2 UCRT64 and update the packages
```shell
pacman -Syu
```
3: Install nesesary tools for the build (you may need to reopen console after this)
```shell
pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-git mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```
4: Clone project's repo
```shell
git clone https://infgotoinf/Sand.git && cd Sand
```
5: Install git modules
```shell
git submodule update --init --recursive
```
6: Generate config
```shell
cmake -B build
```
7: Build executable
```shell
cmake --build build
```
8: Run it
```shell
build/game
```


### Steps to build the test
1: Generate build config with BUILD_TESTING enabled
```shell
cmake -B build -DBUILD_TESTING=ON
```
2: Build the game
```shell
cmake --build build
```
3: Run the test
```shell
build/game_test -d yes
```
or (not recommended)
```shell
ctest --test-dir build
```


### Steps to build the docs
1: Build docs
```shell
doxygen
```
2: Open the docs
- If you're on windows:
Grag and drop docs/index.html on your browser window
- If you're on linux:
```shell
xdg-open docs/index.html
```
