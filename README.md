# A* Demo

## Kompilacja na Windows MinGW

### Wymagania
* CMake >=3.15
* MinGW64 >=5.0.0

```shell
$ cmake -S . -B build -G "MinGW Makefiles"
$ cmake --build build
$ build\Demo.exe
```

## Kompilacja na Linux

### Wymagania
* CMake >=3.15

```shell
$ cmake -S . -B build
$ cmake --build build
$ build\Demo.exe
```