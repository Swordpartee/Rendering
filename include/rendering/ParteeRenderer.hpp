#pragma once

#include <windows.h>      // For HGLRC
#include <GL/glew.h>      // For OpenGL functions and types
#include <GL/gl.h>        // For additional GL types
#include <string>         // For std::string
#include <vector>
#include <memory>

namespace Rendering
{
    class ParteeWindow;   // Forward declaration instead of include
    class ParteeCamera;   // Forward declaration
    class RenderObject;   // Forward declaration
    
    class ParteeRenderer
    {
    public:
        ParteeRenderer(ParteeWindow& window);
        ~ParteeRenderer();

        void initialize();
        void renderScene(const std::vector<std::shared_ptr<RenderObject>>& objects, 
                        const ParteeCamera& camera);
        void clear();
        void present();

        // Shader management
        bool loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
        
        // Texture management
        unsigned int createTexture(int width, int height, const void* data);
        void deleteTexture(unsigned int textureID);

    private:
        ParteeWindow& m_Window;
        HGLRC hrc;
        
        // Shader program
        unsigned int m_shaderProgram;
        
        // Uniform locations (cached for performance)
        int m_modelLoc;
        int m_viewLoc;
        int m_projectionLoc;
        int m_textureLoc;

        // Default texture for objects without textures
        unsigned int m_defaultTexture;

        void initOpenGL();
        void cacheUniformLocations();
        std::string loadShaderFromFile(const std::string& filePath);
        unsigned int compileShader(const std::string& source, GLenum shaderType);
        unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
        void createDefaultTexture();
    };
} // namespace Rendering
