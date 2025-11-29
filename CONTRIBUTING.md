# Contributing to CaffeineForest

Thank you for your interest in contributing! This document provides guidelines and instructions for contributing to the project.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/CaffeineForest.git`
3. Create a feature branch: `git checkout -b feature/your-feature-name`
4. Make your changes
5. Test your changes
6. Submit a pull request

## Code Style

- Use C++17/20 features where appropriate
- Follow RAII principles for resource management
- Prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`) over raw pointers
- Use `const` where possible
- Use meaningful variable and function names
- Add comments for complex algorithms

## Commit Messages

Write clear, descriptive commit messages:
- Use present tense ("Add feature" not "Added feature")
- First line should be a short summary (50 chars or less)
- Add detailed description if needed

Example:
```
Add GPU skinning support

Implements SSBO-based bone matrix upload for efficient
skeletal animation on the GPU. Supports up to 128 bones.
```

## Pull Request Process

1. Ensure your code compiles without warnings
2. Test on at least one platform (Windows, macOS, or Linux)
3. Update documentation if needed
4. Add screenshots/GIFs for visual features
5. Reference any related issues

## Areas for Contribution

- **Renderer**: PBR, shadows, post-processing effects
- **Animation**: Blending, state machines, root motion
- **Physics**: Bullet integration, character controllers
- **Foliage**: LOD improvements, wind animation
- **Tools**: Asset pipeline, editor features
- **Documentation**: Code comments, tutorials, examples

## Questions?

Open an issue for discussion or questions about implementation approaches.

