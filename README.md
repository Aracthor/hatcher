# Hatcher

A C++ API and game engine using Entity-Component-System for both gameplay and rendering update.

## Features

All games created with this API automatically support the following features :

### Save states

Save anytime with **F5**, instantly load with **F9**.

### Deterministic random

Running a game with no argument will generate a random seed. But it is possible to run a specific seed with command line parameter `--seed`.  
Several instances of the same game with the same seed shall have the same random generation.

### Replays

It is possible to save every player command into a save file, to then run a game using those commands to "replay" the game. (Using the same seed is necessary to play the exact same game)  
Specify a file to save commands with `--save` parameter, or a save file to use with `--load` parameter.

For instance, play a game using this command line :
```
./bin/game-name --seed 42 --save replay.hatcher
```
Then rewatch the exact same game by running this command line :
```
./bin/game-name --seed 42 --load replay.hatcher
```

## Demo

Check an online [remake of Asteroids](https://aracthor.github.io/hatcher/asteroids/index.html) as a proof-of-concept, or an [ongoing prototype](https://aracthor.github.io/stevetopia/index.html) for a somewhat more elaborate game.

You can also run a native linux version locally. You shall need glm and SDL2 dev libraries, installable in apt packages :
```
sudo apt install libglm-dev libsdl2-dev
```
You can then compile and run asteroids project:
```
cd asteroids/
make
./bin/asteroids_release
```

## Dependencies
* [OpenGL ES 3.0](https://en.wikipedia.org/wiki/OpenGL_ES) for rendering.
* [LibGLM](https://github.com/icaven/glm) for vector and matrix analysis.
* [SDL 2](https://wiki.libsdl.org/SDL2/FrontPage) for native window creation and event handling.
* [Dear ImGui](https://github.com/ocornut/imgui) for interface widgets.
* [Emscripten](https://emscripten.org/) to compile in websm version running in web browser.
