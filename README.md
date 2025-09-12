# Rendering Engine

A modern C++ rendering engine using OpenGL and GLEW.

## Project Structure

```
Rendering/
├── src/                      # Source files (.cpp)
│   ├── main.cpp             # Application entry point
│   ├── ParteeWindow.cpp     # Window management
│   └── ParteeRenderer.cpp   # OpenGL rendering
├── include/rendering/        # Public headers (.hpp)
│   ├── ParteeWindow.hpp     # Window class interface
│   └── ParteeRenderer.hpp   # Renderer class interface
├── assets/shaders/          # Shader files (.glsl)
│   ├── fragShader.glsl      # Fragment shader
│   └── vertexShader.glsl    # Vertex shader
├── build/                   # Build outputs
├── libs/                    # Third-party libraries
│   └── glew32.lib          # GLEW static library
├── include/GL/             # OpenGL headers
├── .gitignore              # Git ignore rules
├── Makefile                # Build configuration
└── README.md               # This file
```

## Building

### Prerequisites
- MinGW-w64 (g++ compiler)
- GLEW library
- OpenGL development headers

### Build Commands
```bash
# Build the project
mingw32-make

# Clean build artifacts
mingw32-make clean

# Build and run
mingw32-make run

# Rebuild everything
mingw32-make rebuild
```

## Features

- Modern C++ architecture with separation of concerns
- Window management abstraction
- OpenGL rendering engine
- Shader support
- Cross-platform Makefile build system

## Architecture

- **ParteeWindow**: Handles window creation, events, and OS integration
- **ParteeRenderer**: Manages OpenGL context, shaders, and rendering
- **main.cpp**: Application orchestration and main loop

The window and renderer communicate via callback patterns, maintaining clean separation between windowing and graphics code.