#include "ImGuiIntegration.hpp"

#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "gl.hpp"
#include "imgui.h"

namespace hatcher
{

ImGuiIntegration::ImGuiIntegration(SDL_Window* window, SDL_GLContext* glContext)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL; // TODO for emscripten build only ?

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    const char* glsl_version = "#version 300 es";
    ImGui_ImplOpenGL3_Init(glsl_version);
    glGetError(); // ImGui_ImplOpenGL3_Init seems to raise an error, we don't want to catch it...
}

ImGuiIntegration::~ImGuiIntegration() = default;

void ImGuiIntegration::NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    glGetError(); // ImGui::NewFrame seems to raise an error, we don't want to catch it...
}

void ImGuiIntegration::Render() const
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiIntegration::ProcessEvent(const SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

bool ImGuiIntegration::IsInterfaceWindowHovered() const
{
    return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
}

} // namespace hatcher
