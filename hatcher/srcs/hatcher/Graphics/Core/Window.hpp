#pragma once

#include <vector>

#include "hatcher/unique_ptr.hpp"

struct SDL_Window;
union SDL_Event;

namespace hatcher
{
class GLContext;
class ImGuiIntegration;

class Window
{
public:
    Window(const char* name, int width, int height);
    ~Window();

    void Clear();
    void Refresh();

    std::vector<SDL_Event> PollEvents();

    GLContext* Context() const { return m_context.get(); }

    int Width() const { return m_width; }
    int Height() const { return m_height; }

private:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    unique_ptr<GLContext> m_context;
    unique_ptr<ImGuiIntegration> m_imguiIntegration;
};

} // namespace hatcher
