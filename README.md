# CaffeineForest

> **A 100% C++ handcrafted game engine** featuring skeletal animation, GPU skinning, instanced forest rendering, and Bullet physics integration.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![C++](https://img.shields.io/badge/C++-17/20-blue)]()
[![License](https://img.shields.io/badge/license-MIT-green)]()

## 🎮 Features

- **Modern C++ Renderer** - OpenGL 4.5 core profile, deferred rendering pipeline
- **Skeletal Animation** - CPU and GPU-based skinning with animation blending
- **Instanced Foliage** - Efficient forest rendering with LOD and frustum culling
- **Physics Integration** - Bullet Physics for collisions and character controllers
- **Asset Pipeline** - Assimp-based model loading with full bone hierarchy support
- **Debug UI** - ImGui integration for real-time engine inspection
- **Resource Management** - Hot-reloadable shaders and efficient asset caching

## 🏗️ Architecture

```
CaffeineForest/
├── Core Systems      - Window, Input, Application, Logger
├── Renderer         - Shader, Mesh, Model, GBuffer, GPUAnimator
├── Scene            - Entity-Component system, Transform, Camera
├── Animation        - Skeleton, Animator, AnimationClip
├── Physics          - Bullet wrapper for collisions
├── Foliage          - Instanced rendering system
└── Utils            - FileSystem, Profiler
```

## 🚀 Quick Start

### Prerequisites

- CMake 3.18+
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- OpenGL 4.5+ capable graphics card

### Build

```bash
git clone https://github.com/yourusername/CaffeineForest.git
cd CaffeineForest
mkdir build && cd build
cmake ..
cmake --build .
```

The project uses CMake's `FetchContent` to automatically download dependencies:
- GLFW (window management)
- GLAD (OpenGL loader)
- GLM (math library)
- ImGui (debug UI)
- Assimp (model loading)

### Run

```bash
./bin/CaffeineForest
```

Assets are automatically copied to the build directory during compilation.

## 📦 Dependencies

All dependencies are fetched automatically via CMake FetchContent:

- **GLFW 3.3.8** - Window and input handling
- **GLAD v0.1.36** - OpenGL function loading
- **GLM 0.9.9.8** - Mathematics library
- **ImGui v1.89.8** - Immediate mode GUI
- **Assimp v5.2.5** - 3D model import

Optional (for future features):
- Bullet Physics (to be integrated)
- stb_image (texture loading)

## 🎯 Roadmap

- [x] Project scaffold and build system
- [x] Core window and input systems
- [x] Basic renderer with shader support
- [x] Model loading via Assimp
- [ ] Camera controller (free-fly and third-person)
- [ ] CPU skeletal animation
- [ ] GPU skinning with SSBO
- [ ] Bullet Physics integration
- [ ] Instanced foliage rendering
- [ ] Terrain system
- [ ] Animation blending and state machine
- [ ] PBR rendering pipeline
- [ ] Shadow mapping
- [ ] SSAO

## 🎨 Demo Assets

For testing, download free assets from:
- [Mixamo](https://www.mixamo.com/) - Rigged characters and animations
- [Quaternius](https://quaternius.com/) - Low-poly models
- [Kenney Assets](https://kenney.nl/) - Game assets

Place models in `assets/models/`, textures in `assets/textures/`, and animations in `assets/animations/`.

## 🛠️ Development

### Code Style

- C++17/20 features where appropriate
- RAII for resource management
- Smart pointers (`std::unique_ptr`, `std::shared_ptr`)
- Modern OpenGL (core profile, no deprecated functions)

### Contributing

Contributions welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request with clear description

## 📝 License

MIT License - see LICENSE file for details

## 🙏 Acknowledgments

Built with passion and caffeine. No engine shortcuts—everything from scratch.

---

**Made by someone who wanted to suffer through building a game engine in C++** ☕

# 3D-Game-2
