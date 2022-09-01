#pragma once

struct SDL_Window;
typedef void* SDL_GLContext;

namespace hatcher
{

class ImGuiIntegration
{
public:
    ImGuiIntegration(SDL_Window* window, SDL_GLContext* glContext);
    ~ImGuiIntegration();

    void NewFrame();
    void Render() const;
};

} // namespace hatcher
