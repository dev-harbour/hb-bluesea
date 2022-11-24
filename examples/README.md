# hb-bluesea

## Installing on Windows

- MSYS2

   ```
   pacman -S mingw-w64-x86_64-glfw
   pacman -S mingw-w64-x86_64-cairo
   pacman -S mingw-w64-x86_64-freetype
   ```

   ```
   export HB_WITH_FREETYPE=/mingw64/include/freetype2
   ```

   To test it, type:

   ```
   $ cd examples/main
   $ hbmk2 main.prg
   ```

## Installing on Linux

- Debian

   ```
   sudo apt-get update
   sudo apt-get install libglfw3-dev
   sudo apt-get install libcairo2-dev
   sudo apt-get install libfreetype6-dev
   ```
   To test it, type:

   ```
   $ cd examples/main
   $ hbmk2 main.prg

   ```
---
