#pragma once

#include <memory>

struct SDL_Window;

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

    void EnableDepthTest();
    void DisableDepthTest();

    int Width() const { return m_width; }
    int Height() const { return m_height; }

private:
    int m_width;
    int m_height;
    SDL_Window* m_window;
    std::unique_ptr<GLContext> m_context;
    std::unique_ptr<ImGuiIntegration> m_imguiIntegration;
};

} // namespace hatcher
