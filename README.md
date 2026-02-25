# Sand
Мы кароче делаем лютейшую игру на С/С++ тупо на SDL3 как сигмы наилютейшие, да.

## Steps to build
1: Get a copy of SDL source
```shell
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
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
