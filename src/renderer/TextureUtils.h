#pragma once

// Utility functions for texture creation
class TextureUtils {
public:
    // Create a 1x1 white texture (useful for untextured meshes)
    static unsigned int CreateWhiteTexture();
    
    // Create a 1x1 colored texture
    static unsigned int CreateColorTexture(float r, float g, float b, float a = 1.0f);
    
    // Create a checkerboard pattern texture
    static unsigned int CreateCheckerboardTexture(int size = 64, int squares = 8);
};

