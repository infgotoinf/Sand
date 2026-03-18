# Sand
[Wiki](https://github.com/infgotoinf/Sand/wiki/О-проекте)

## Steps to build
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
cmake -S . -B build
```
3: Build executable
```shell
cmake --build build
```
4: Run it
```shell
build/sand
```

## Steps to build the docs
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
