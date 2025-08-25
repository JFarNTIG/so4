# Spaced Out 4

Copyright (C) 2019-2025 Jacob Farnsworth

Spaced Out 4 is a 2D, top-down space-themed action-adventure game with RPG elements. It is inspired by the Spaced Out series of games by Sean Hunter and intended as a spiritual successor.

The game is written primarily in C++, using SFML for graphics, audio and rendering, as well as Dear ImGUI for UI. SO4 is in a pre-alpha state; everything you see is subject to change, and lots of it.

SO4 is intended to be heavily moddable, with most game data and settings stored in INI files. In addition, the game has a Lua script processor, and missions, systems, and events are represented as Lua scripts.

## Considerations

Seeing as the game is in a pre-alpha state, many of the assets (primarily graphics and audio assets) are temporary, and taken from various freely available collections found on the Web.

I've attempted to credit as many authors as I can. The credits list can be found in the credits folder.

IMPORTANT: If you notice that your work is included in this repository, and your name was not properly recorded in the credits file, don't hesitate to contact me so that I can credit you for your work.

## Compiling

It is recommended to build Spaced Out 4 using mingw-w64.

Install MSYS2 for Windows: https://www.msys2.org/

In the MSYS2 terminal:

Install platform toolchain and CMake:

```
pacman -S --needed base-devel mingw-w64-x86_64-toolchain cmake ninja
```

Install dependencies (SFML, FreeType, Lua, glew, and freeglut):

```
pacman -S --needed \
    mingw-w64-x86_64-sfml \
    mingw-w64-x86_64-freetype \
    mingw-w64-x86_64-lua \
    mingw-w64-x86_64-glew \
    mingw-w64-x86_64-freeglut
```

To build the project from the command line:

```
mkdir build && cd build
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..
ninja
```

Under development, you can use build type=Debug to build with debug symbols.

If you're using Visual Studio Code, you can easily configure and build using the interface in the CMake plugin.

### Packaging

The assets folder needs to be included in any packaging or deployment of SO4.

Also, the files: imgui.ini and settings.ini should be included.

The game will look for settings.ini in the current working directory.

In settings.ini, the `data_directory` entry should point to the assets folder, relative from the working directory when the game is run.

## Contributing

Take a look at CONTRIBUTING.md for more information.

## License

SO4 is published under the terms of the GNU GPLv2.
