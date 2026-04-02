# Sand
[Wiki](https://github.com/infgotoinf/Sand/wiki/О-проекте)

## Build
### Steps to build on linux
0: If you're on nixos - enter the dev shell
```shell
nix-shell
```
1: Install git modules
```shell
git submodule update --init --recursive
```
2: Generate config
```shell
cmake -B build
```
3: Build executable
```shell
cmake --build build
```
4: Run it
```shell
build/sand
```

### Steps to build on windows
0: If you have balls
> Uninstall windows and intall linux to follow linux build steps

1: Install MSYS2 https://github.com/msys2/msys2-installer/releases

2: Launch MSYS2 UCRT64 and update the packages
```shell
pacman -Syu
```
3: Install nesesary tools for the build (you may need reopen console after this)
```shell
pacman -S mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-git mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```
4: Install git modules
```shell
git submodule update --init --recursive
```
5: Generate config
```shell
cmake -B build
```
6: Build executable
```shell
cmake --build build
```
7: Run it
```shell
build/sand
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
ctest --test-dir build
```

### Steps to build the docs
1: Build docs
```shell
doxygen
```
2: Open the docs
- If you're on windows:
> Uninstall windows and istall linux instead
- If you're on linux:
```shell
xdg-open docs/html/index.html
```
