## Demo Code of SDL2 + Box2D

Modified from [here](https://github.com/eder13/Box2D-SDL2-Demo), convert it to an OO version.

This code shows some basic methods to convert vectors from the SDL2 coordinate system to box2d's and also the reverse direction.

### build (macOS)

I usually install packages from brew:

```bash
brew install box2d sdl2
```

Inside the project dir, run:

```bash
mkdir build; cd build
cmake ..; make -j
```



