# Sand
[Wiki](https://github.com/infgotoinf/Sand/wiki/О-проекте)

## Steps to build
0: If you're on nixos - enter the dev shell
```shell
nix-shell
```
1: Generate config
```shell
cmake -S . -B build
```
2: Build executable
```shell
cmake --build build
```
3: Run it
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
