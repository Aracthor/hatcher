#pragma once

#include <memory>

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

    int Width() const { return m_width; }
    int Height() const { return m_height; }

    SDL_Window* window() { return m_window; }

private:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    std::unique_ptr<GLContext> m_context;
};

} // namespace hatcher
