//
// Created by Akira Mujawar on 08/07/24.
//

#include <SDL_video.h>
#include <log.hpp>
#include "imgui_wrapper.hpp"
#include "imgui_renderer.hpp"

using physicat::graphics::ImGuiRenderer;

ImGuiRenderer::ImGuiRenderer(SDL_Window* window, SDL_GLContext& context) {
    physicat::Log("ImGuiRenderer","Creating...");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init("#version 400");
}

ImGuiRenderer::~ImGuiRenderer() {
    physicat::Log("ImGuiRenderer","Destroying...");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRenderer::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    bool show;
    ImGui::ShowDemoWindow(&show);
}

void ImGuiRenderer::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::Input(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
}


