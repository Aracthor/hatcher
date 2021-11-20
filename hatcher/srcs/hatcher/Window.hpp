#pragma once

class SDL_Window;

namespace hatcher
{
class GLContext;

class Window
{
public:
    Window(const char* name, int width, int height);
    ~Window();

    void Refresh();

private:
    SDL_Window* m_window;
    GLContext* m_context;
};

} // namespace hatcher
