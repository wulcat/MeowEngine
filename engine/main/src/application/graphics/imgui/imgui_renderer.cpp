//
// Created by Akira Mujawar on 08/07/24.
//

#include <SDL_video.h>
#include <SDL_events.h>

#include <sys/wait.h> // For waitpid()
#include <unistd.h> // For fork(), exec()
#include <csignal> // For signal handling

#include <log.hpp>
#include "imgui_wrapper.hpp"
#include "imgui_renderer.hpp"
#include "bridge_wrapper.hpp"

using physicat::graphics::ImGuiRenderer;

namespace {
    pid_t tracy_profiler_pid = -1;

    static void HandleTracyProfilerSignal(int signal) {
        if (tracy_profiler_pid > 0) {
            kill(tracy_profiler_pid, SIGTERM); // Send termination signal to child
            waitpid(tracy_profiler_pid, nullptr, 0); // Wait for child to terminate
        }
        exit(0); // Exit the parent process
    }
}
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

#ifdef __APPLE__
    createMacMenu();
#elif _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    createWindowsMenu(wmInfo.info.win.window);
#endif
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
//    ImGui::Begin("Menu");
//    if (ImGui::Button("Option 1")) {
////        std::cout << "Option 1 selected" << std::endl;
//    }
//    if (ImGui::Button("Option 2")) {
////        std::cout << "Option 2 selected" << std::endl;
//    }
//    if (ImGui::Button("Option 3")) {
////        std::cout << "Option 3 selected" << std::endl;
//    }
//    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::Input(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);

    // Handles custom SDL events
#ifdef _WIN32
    if (event.type == SDL_SYSWMEVENT) {
        if (event.syswm.msg->msg.win.msg == WM_COMMAND) {
            switch (LOWORD(event.syswm.msg->msg.win.wParam)) {
                case 1:
                    std::cout << "Option 1 selected" << std::endl;
                    break;
            }
        }
    }
#endif

#ifdef __APPLE__
    if (event.type == SDL_USEREVENT) {
        switch (event.user.code) {
            case 1:
                OpenTracyProfiler();
                break;
        }
    }
#endif
}

void ImGuiRenderer::OpenTracyProfiler() {
    // Register signal handlers to clean up child process on exit
    signal(SIGINT, &::HandleTracyProfilerSignal);  // Handle Ctrl+C
    signal(SIGTERM, &::HandleTracyProfilerSignal); // Handle termination signals
    signal(SIGQUIT, &::HandleTracyProfilerSignal); // Handle quit signals

    tracy_profiler_pid = fork();

    if (tracy_profiler_pid == -1) {
        perror("fork");
        exit(1);
    }

    if (tracy_profiler_pid == 0) {
        // Child process
        const char* path = "profiler/build/unix/Tracy-release";
        if (execl(path, path, (char*)NULL) == -1) {
            perror("execl");
            exit(1);
        }
    }
}

void ImGuiRenderer::ClosePIDs() {
    ::HandleTracyProfilerSignal(SIGQUIT);
}

