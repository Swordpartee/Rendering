#pragma once

#include <windows.h>      // For HGLRC
#include <GL/glew.h>      // For OpenGL functions and types
#include <GL/gl.h>        // For additional GL types
#include <string>         // For std::string

namespace Rendering
{
    class ParteeWindow;   // Forward declaration instead of include
    
    class ParteeRenderer
    {
    public:
        ParteeRenderer(ParteeWindow& window);
        ~ParteeRenderer();

        void render();

    private:
        ParteeWindow& m_Window;
        HGLRC hrc;
        GLuint textureID;
        GLubyte* pixelData;
        GLuint shaderProgram;
        GLuint VAO, VBO;
        void initOpenGL();
        std::string loadShaderFromFile(const std::string& filePath);
    };
} // namespace Rendering
