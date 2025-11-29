#pragma once

#include <glm/glm.hpp>

class Renderer {
public:
    static void Init();
    static void Shutdown();

    static void SetClearColor(const glm::vec4& color);
    static void Clear();
    static void SetViewport(int x, int y, int width, int height);
};

