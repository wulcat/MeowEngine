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
ImGuiRenderer::ImGuiRenderer(SDL_Window* window, SDL_GLContext& context)
    : isSceneViewportFocused(false)
    , SceneViewportSize({0,0}) {

    physicat::Log("ImGuiRenderer","Creating...");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

void ImGuiRenderer::Input(const SDL_Event& event) {
    PT_PROFILE_SCOPE_N("UI Input");;

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

void physicat::graphics::ImGuiRenderer::Render(unsigned int frameBufferId) {
    CreateNewFrame();
    DrawFrame(frameBufferId);
    RenderFrame();
}

void ImGuiRenderer::ClosePIDs() {
    ::HandleTracyProfilerSignal(SIGQUIT);
}

bool physicat::graphics::ImGuiRenderer::IsSceneViewportFocused() const {
    return isSceneViewportFocused;
}

const physicat::WindowSize& physicat::graphics::ImGuiRenderer::GetSceneViewportSize() const {
    return SceneViewportSize;
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

void physicat::graphics::ImGuiRenderer::CreateNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void physicat::graphics::ImGuiRenderer::DrawFrame(unsigned int frameBufferId) {
//    ImGui::DockSpaceOverViewport
    bool show;
    ImGui::ShowDemoWindow(&show);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
//    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
//    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
//    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
//    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
//    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
//    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
//    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
//    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
//    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
//    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
//    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin
    bool test;
    ImGui::Begin("Scene", &test,window_flags);
    {
        isSceneViewportFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        ImGui::BeginChild("GameRender");

        const ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if(viewportSize.x != SceneViewportSize.width || SceneViewportSize.height != SceneViewportSize.height) {
            SceneViewportSize.width = viewportSize.x;
            SceneViewportSize.height = viewportSize.y;

            SDL_Event event;
            SDL_zero(event);
            event.type = SDL_USEREVENT;
            event.user.code = 2;
            SDL_PushEvent(&event);
        }

        ImGui::Image(
                (ImTextureID)frameBufferId,
                ImGui::GetContentRegionAvail(),
                ImVec2(0, 1),
                ImVec2(1, 0)
        );
    }
    ImGui::EndChild();
    ImGui::End();
}

void physicat::graphics::ImGuiRenderer::RenderFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
