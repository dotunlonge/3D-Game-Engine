#pragma once

class ImGuiLayer {
public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void Begin();
    void End();

    void OnImGuiRender();
};

