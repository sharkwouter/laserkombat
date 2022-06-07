# Building

This document contains information on how to build Laser Kombat for each supported platform.

## Ubuntu

On Ubuntu install the required packages with the following command:

```
sudo apt-get install git cmake build-essential pkgconf libsdl2-dev libsdl2-ttf-dev
```

clone the git repository (to get the code):
```
git clone https://github.com/sharkwouter/laserkombat.git
```

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir build
cd build
cmake ..
make
```

Then the game can be started with:

```
./laserkombat
```

The executable should be found in ``~/laserkombat/build``.

## Windows

On Windows first download and install [msys2](https://www.msys2.org/). Then start the MinGW64 terminal and install the following packages with it:

```
pacman -Syu git mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils mingw-w64-x86_64-make mingw-w64-x86_64-cmake mingw-w64-x86_64-pkgconf mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf
```

clone the git repository (to get the code):
```
git clone https://github.com/sharkwouter/laserkombat.git
```

To build execute the following commands from a terminal in the directory where the code is found:

```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
```

Then the game can be started through ``laserkombat.exe`. It should be in ``C:\msys2\mingw64\home\YOUR-USERNAME\laserkombat\build``.

## Other platforms

It is possible to build for Vita and Xbox with their respective homebrew SDKs. These ports are not finished or supported at the moment, though. Please submit an issue if you'd like to work on them.
