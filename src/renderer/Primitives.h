#pragma once

#include "Mesh.h"
#include <vector>

// Simple primitive mesh generators
class Primitives {
public:
    static Mesh CreateCube();
    static Mesh CreateQuad();
    static Mesh CreateSphere(int segments = 32);
    static Mesh CreatePlane(float width = 1.0f, float height = 1.0f);
};

