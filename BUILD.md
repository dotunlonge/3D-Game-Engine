# Build Instructions

## Prerequisites

- CMake 3.18 or higher
- C++17 compatible compiler
- Python 3 (for GLAD loader generation)
- OpenGL 4.5+ capable graphics card

## Setting up GLAD

GLAD (OpenGL loader) needs to be generated before building. Run:

```bash
pip install glad
glad --generator=c --spec=gl --api=gl=4.5 --profile=core --out-path=src/glad
```

This will create `src/glad/src/glad.c` and `src/glad/include/glad/glad.h`.

Alternatively, download a pre-generated GLAD loader from https://glad.dav1d.de/ and place it in `src/glad/`.

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The executable will be in `build/bin/CaffeineForest`.

## Dependencies

All dependencies are automatically fetched via CMake FetchContent:
- GLFW 3.3.8
- GLM 0.9.9.8
- ImGui v1.89.8
- Assimp v5.2.5
- stb_image (header-only)

## Troubleshooting

### GLAD not found
Make sure you've generated GLAD as described above, or update `CMakeLists.txt` to point to your GLAD installation.

### Assimp build fails
Assimp can take a while to build. If it fails, try:
- Using vcpkg: `vcpkg install assimp`
- Building Assimp separately and pointing CMake to it

### OpenGL not found (Linux)
Install OpenGL development packages:
```bash
sudo apt-get install libgl1-mesa-dev  # Ubuntu/Debian
sudo yum install mesa-libGL-devel     # Fedora/RHEL
```

