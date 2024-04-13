## 🥥 Coconut Engine

Coconut Engine is a 2D game engine currently under development.

### build (macOS)

This engine is temporarily build for MacOS only. If you have not installed brew, install brew first. Then install the dependencies:

```bash
brew install sdl spdlog box2d sdl_image yaml-cpp
```

Inside the project dir, run:

```bash
mkdir build; cd build
cmake ..; make -j
```


