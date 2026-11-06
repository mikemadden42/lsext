# lsext
Show files by extension

## Usage

```
lsext [directory]
```

Lists the regular files in `directory`, or the current directory if none is
given, grouped by extension. Use `./-name` for a directory starting with `-`.

## Build

Building requires a C++17 compiler. The commands below require CMake 3.20 or
later.

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

## Install

```
cmake --install build --prefix ~/.local
```

This installs `lsext` to `~/.local/bin`. Without `--prefix`, it installs to
`/usr/local/bin`, which may require `sudo`.
