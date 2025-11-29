# Changelog

## [Unreleased]

### Added
- **Application Architecture**: Complete application class with layer system
- **Resource Manager**: Centralized shader and texture loading with caching
- **Game Layer System**: Extensible layer architecture for game/UI separation
- **Enhanced ImGui Integration**: Improved debug UI with performance metrics
- **GitHub Actions CI**: Automated builds for Linux, Windows, and macOS
- **Example GameLayer**: Demonstration of how to use the engine systems
- **Enhanced Renderer**: Better initialization with OpenGL info logging
- **Layer Management**: Push/pop layer system for modular architecture

### Changed
- **main.cpp**: Now uses Application class instead of direct GLFW/ImGui calls
- **Application**: Enhanced with layer stack and proper render pipeline
- **ImGuiLayer**: Expanded debug information display

### Architecture Improvements
- Separation of concerns: Core, Renderer, Scene, Animation systems
- Resource management: Centralized loading and caching
- Layer system: Modular game/UI layer architecture
- Better error handling and logging throughout

### Next Steps
- Complete skeletal animation implementation
- Add Bullet Physics integration
- Implement instanced foliage rendering
- Add PBR material system
- Create example scenes with models

