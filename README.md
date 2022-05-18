# Password Manager

[![linux](https://github.com/tqfx/password-manager/actions/workflows/linux.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/linux.yml)
[![mingw](https://github.com/tqfx/password-manager/actions/workflows/mingw.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/mingw.yml)
[![msys2](https://github.com/tqfx/password-manager/actions/workflows/msys2.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/msys2.yml)
[![freebsd](https://github.com/tqfx/password-manager/actions/workflows/freebsd.yml/badge.svg)](https://github.com/tqfx/password-manager/actions/workflows/freebsd.yml)

## prepare

Set the environment variable `VCPKG_INSTALLATION_ROOT` to point to [vcpkg](https://github.com/Microsoft/vcpkg), such as `VCPKG_INSTALLATION_ROOT=vcpkg`.

## release

```bash
cmake -S . -B build
cmake --build build
```

## mingw on linux

### install

```bash
sudo apt-get install -y cmake ninja-build zip unzip curl git pkg-config mingw-w64
```

### mingw32

```bash
cmake -S . -B build -DVCPKG_TARGET_TRIPLET=x86-mingw-static
cmake --build build
```

### mingw64

```bash
cmake -S . -B build -DVCPKG_TARGET_TRIPLET=x64-mingw-static
cmake --build build
```

## mingw on windows

### install [tdm-gcc](https://jmeubank.github.io/tdm-gcc/download) or [winlibs](https://winlibs.com) or [msys2](https://www.msys2.org)

### mingw32

```bash
cmake -S . -B build -G "MinGW Makefiles" -DVCPKG_TARGET_TRIPLET=x86-mingw-static
cmake --build build
```

### mingw64

```bash
cmake -S . -B build -G "MinGW Makefiles" -DVCPKG_TARGET_TRIPLET=x64-mingw-static
cmake --build build
```

## termux

```bash
pkg install -y clang cmake ninja zip unzip curl git pkg-config
```

```bash
cmake -S . -B build -DVCPKG_TARGET_TRIPLET=arm64-linux
cmake --build build
```
