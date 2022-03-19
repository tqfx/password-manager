# Password Manager

[![linux](https://github.com/tqfx/password-manager/actions/workflows/linux.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/linux.yml)
[![mingw](https://github.com/tqfx/password-manager/actions/workflows/mingw.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/mingw.yml)
[![msys2](https://github.com/tqfx/password-manager/actions/workflows/msys2.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/msys2.yml)
[![freebsd](https://github.com/tqfx/password-manager/actions/workflows/freebsd.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/freebsd.yml)

## prepare

Set the environment variable `VCPKG_INSTALLATION_ROOT` to point to [vcpkg](https://github.com/Microsoft/vcpkg), such as `VCPKG_INSTALLATION_ROOT=vcpkg`.

## release

```
cmake --preset=release
cmake --build --preset=release
```

## mingw on linux

### install

```bash
sudo apt-get install -y cmake ninja-build zip unzip curl git pkg-config mingw-w64
```

### mingw32

```
cmake --preset=mingw32
cmake --build --preset=mingw32
```

### mingw64

```
cmake --preset=mingw64
cmake --build --preset=mingw64
```

## mingw on windows

### install [tdm-gcc](https://jmeubank.github.io/tdm-gcc/download) or [winlibs](https://winlibs.com) or [msys2](https://www.msys2.org)

### mingw32

```
cmake --preset=mingw32 -G "MinGW Makefiles"
cmake --build --preset=mingw32
```

### mingw64

```
cmake --preset=mingw64 -G "MinGW Makefiles"
cmake --build --preset=mingw64
```

## termux

```
pkg install -y clang cmake ninja zip unzip curl git pkg-config
```

```
cmake --preset=termux
cmake --build --preset=termux
```
