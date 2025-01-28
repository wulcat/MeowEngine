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
#include "imgui_user_interface_system.hpp"
#include "bridge_wrapper.hpp"
//#include "entt_wrapper.hpp"
//#include <filesystem>

using MeowEngine::graphics::ImGuiUserInterfaceSystem;

namespace {
    pid_t tracy_profiler_pid = -1;

    static void HandleTracyProfilerSignal(int signal) {
        if (tracy_profiler_pid > 0) {
            kill(tracy_profiler_pid, SIGTERM); // Send termination signal to child
            waitpid(tracy_profiler_pid, nullptr, 0); // Wait for child to terminate
        }
//        exit(0); // Exit the parent process // shouldn't do this or the application destruction won't happen
    }

    void LoadIniFromFileSystem() {
        const char* iniPath = "assets/layout.ini"; // Path in the virtual filesystem
        if (FILE* file = fopen(iniPath, "r")) {
            fseek(file, 0, SEEK_END);
            size_t size = ftell(file);
            rewind(file);

            char* buffer = new char[size + 1];
            fread(buffer, 1, size, file);
            buffer[size] = '\0'; // Null-terminate the string
            fclose(file);

            ImGui::LoadIniSettingsFromMemory(buffer, size);
            delete[] buffer;

            MeowEngine::Log("ImGUI", "Loaded File");
        }
        else {
            MeowEngine::Log("ImGUI", "Failed to open file");
        }
    }
}


ImGuiUserInterfaceSystem::ImGuiUserInterfaceSystem(SDL_Window* window, SDL_GLContext& context)
//    : isSceneViewportFocused(false)
//    , SceneViewportSize({0,0})
    : StructurePanel()
    , WorldRenderPanel()
    , LogPanel() {

    MeowEngine::Log("ImGuiRenderer","Creating...");

    #ifdef __EMSCRIPTEN__
        const char* glsl_version = "#version 300 es";
    #else
        const char* glsl_version = "#version 400";
    #endif

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

#ifdef __EMSCRIPTEN__
    LoadIniFromFileSystem();
#else
    io.IniFilename = "assets/layout.ini";
    ImGui::LoadIniSettingsFromDisk(io.IniFilename);
#endif

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);

#ifdef __APPLE__
    createMacMenu();
#elif _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    createWindowsMenu(wmInfo.info.win.window);
#endif
}

ImGuiUserInterfaceSystem::~ImGuiUserInterfaceSystem() {
    MeowEngine::Log("ImGuiRenderer","Destroying...");

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiUserInterfaceSystem::Input(const SDL_Event& event) {
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

void MeowEngine::graphics::ImGuiUserInterfaceSystem::Render(entt::registry& registry, std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& inUIInputQueue, unsigned int frameBufferId, const double fps) {
    CreateNewFrame();
    DrawFrame(registry, inUIInputQueue, frameBufferId, fps);
    RenderFrame();
}

void ImGuiUserInterfaceSystem::ClosePIDs() {
    ::HandleTracyProfilerSignal(SIGQUIT);
}

//bool MeowEngine::graphics::ImGuiRenderer::IsSceneViewportFocused() const {
//    return isSceneViewportFocused;
//}
//
//const MeowEngine::WindowSize& MeowEngine::graphics::ImGuiRenderer::GetSceneViewportSize() const {
//    return SceneViewportSize;
//}

void ImGuiUserInterfaceSystem::OpenTracyProfiler() {
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
        const char* path = "dependencies/profiler/build/unix/Tracy-release";
        if (execl(path, path, (char*)NULL) == -1) {
            perror("execl");
            exit(1);
        }
    }
}

void MeowEngine::graphics::ImGuiUserInterfaceSystem::CreateNewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MeowEngine::graphics::ImGuiUserInterfaceSystem::DrawFrame(entt::registry& registry, std::queue<std::shared_ptr<MeowEngine::ReflectionPropertyChange>>& inUIInputQueue, uint32_t frameBufferId, const double fps) {
    CreateDockingSpace();

//    CreateRender3DPanel(frameBufferId);
//    CreateLifeObjectSelectorPanel(scene);


    EditPanel.Draw(registry, inUIInputQueue, StructurePanel.GetSelectedItem());
    StructurePanel.Draw(registry);
    WorldRenderPanel.Draw(reinterpret_cast<void*>(frameBufferId), fps);
    LogPanel.Draw();

//    CreateObjectEditorPanel(temp);
//    CreateLogPanel();

//    ImGui::ShowDemoWindow(&IsRendering);
}

void ImGuiUserInterfaceSystem::RenderFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiUserInterfaceSystem::CreateDockingSpace() {
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
//    if (opt_fullscreen)
    if(true)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
//    if (!opt_padding) {
//        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
//    }

    ImGui::Begin("DockSpace Demo", &IsRendering, window_flags); {
//        if (!opt_padding)
//            ImGui::PopStyleVar();
//
//        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
//                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
//                ImGui::MenuItem("Padding", NULL, &opt_padding);
//                ImGui::Separator();
//
//                if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
//                if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
//                if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
//                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
//                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
//                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
//                ImGui::Separator();
//
//                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
//                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
}