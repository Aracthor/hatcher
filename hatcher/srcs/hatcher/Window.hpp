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

    void Clear();
    void Refresh();

private:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    GLContext* m_context;
};

} // namespace hatcher
