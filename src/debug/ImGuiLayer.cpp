#include "ImGuiLayer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../core/Window.h"
#include "../core/Application.h"
#include "../utils/Profiler.h"

ImGuiLayer::ImGuiLayer() {
}

ImGuiLayer::~ImGuiLayer() {
}

void ImGuiLayer::OnAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::StyleColorsDark();

    Application& app = Application::Get();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

    OnImGuiRender();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnImGuiRender() {
    // Main debug window
    ImGui::Begin("CaffeineForest Debug");
    ImGui::Text("Engine Status: Running");
    ImGui::Separator();
    
    // Performance
    ImGui::Text("Performance");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    
    ImGui::Separator();
    
    // System info
    ImGui::Text("System");
    Application& app = Application::Get();
    ImGui::Text("Window: %dx%d", app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
    
    ImGui::Separator();
    
    // Controls
    ImGui::Text("Controls");
    ImGui::BulletText("ESC: Exit");
    ImGui::BulletText("WASD: Move (when implemented)");
    ImGui::BulletText("Mouse: Look around (when implemented)");
    
    ImGui::End();
    
    // Profiler window (if profiling is active)
    if (ImGui::Begin("Profiler")) {
        // Add profiler data here when implemented
        ImGui::Text("Profiler data will appear here");
        ImGui::End();
    }
}
