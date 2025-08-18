# Hatcher

A C++ game engine using Entity-Component-System for both gameplay and rendering update.

It uses:
* [OpenGL ES 3.0](https://en.wikipedia.org/wiki/OpenGL_ES) for rendering.
* [LibGLM](https://github.com/icaven/glm) for vector and matrix analysis.
* [SDL 2](https://wiki.libsdl.org/SDL2/FrontPage) for native window creation and event handling.
* [Dear ImGui](https://github.com/ocornut/imgui) for interface widgets.
* [Emscripten](https://emscripten.org/) to compile in websm version running in web browser.

Save-states and deterministic replays are handled by design for every application made with this engine.

Check a [remake of Asteroids](https://aracthor.github.io/hatcher/asteroids/index.html) as a proof-of-concept, or an [ongoing prototype](https://aracthor.github.io/hatcher/unnamed_proto/index.html) for a somewhat more elaborate game.
