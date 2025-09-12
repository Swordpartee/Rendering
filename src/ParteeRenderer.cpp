#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/ParteeRenderer.hpp"
#include "rendering/ParteeWindow.hpp"
#include "rendering/ParteeCamera.hpp"
#include "rendering/RenderObject.hpp"

namespace Rendering
{
    ParteeRenderer::ParteeRenderer(ParteeWindow& window) : m_Window(window), hrc(nullptr), 
        m_shaderProgram(0), m_defaultTexture(0)
    {
    }

    ParteeRenderer::~ParteeRenderer()
    {
        if (hrc)
        {
            wglMakeCurrent(m_Window.getHDC(), hrc);

            // Clean up OpenGL objects
            if (m_shaderProgram != 0)
            {
                glDeleteProgram(m_shaderProgram);
            }
            if (m_defaultTexture != 0)
            {
                glDeleteTextures(1, &m_defaultTexture);
            }

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hrc);
        }
    }

    void ParteeRenderer::initialize()
    {
        initOpenGL();
        loadShaders("assets/shaders/vertexShader.glsl", "assets/shaders/fragShader.glsl");
        createDefaultTexture();
        cacheUniformLocations();
    }

    void ParteeRenderer::initOpenGL()
    {
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            32,               // Color depth
            0, 0, 0, 0, 0, 0, // Color bits (ignored)
            0,                // No alpha buffer
            0,                // Alpha bits (ignored)
            0,                // No accumulation buffer
            0, 0, 0, 0,       // Accum bits (ignored)
            24,               // Depth buffer
            8,                // Stencil buffer
            0,                // No auxiliary buffers
            PFD_MAIN_PLANE,   // Main layer
            0,                // Reserved
            0, 0, 0           // Layer masks (ignored)
        };

        int pixelFormat = ChoosePixelFormat(m_Window.getHDC(), &pfd);
        if (!pixelFormat)
        {
            MessageBoxW(NULL, L"Failed to choose pixel format", L"Error", MB_OK);
            return;
        }

        if (!SetPixelFormat(m_Window.getHDC(), pixelFormat, &pfd))
        {
            MessageBoxW(NULL, L"Failed to set pixel format", L"Error", MB_OK);
            return;
        }

        hrc = wglCreateContext(m_Window.getHDC());
        if (!hrc)
        {
            MessageBoxW(NULL, L"Failed to create OpenGL context", L"Error", MB_OK);
            return;
        }
        if (!wglMakeCurrent(m_Window.getHDC(), hrc))
        {
            MessageBoxW(NULL, L"Failed to make context current", L"Error", MB_OK);
            return;
        }

        // Initialize GLEW
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            MessageBoxW(NULL, L"Failed to initialize GLEW", L"Error", MB_OK);
            return;
        }

        // Enable depth testing for 3D
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Set up viewport
        glViewport(0, 0, m_Window.getWidth(), m_Window.getHeight());
    }

    void ParteeRenderer::renderScene(const std::vector<std::shared_ptr<RenderObject>>& objects, 
                                    const ParteeCamera& camera)
    {
        if (m_shaderProgram == 0)
            return;

        glUseProgram(m_shaderProgram);

        // Set view and projection matrices (these are the same for all objects)
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        
        glUniformMatrix4fv(m_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(m_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Render each object
        for (const auto& object : objects)
        {
            if (!object)
                continue;

            // Set model matrix for this object
            glm::mat4 model = object->getModelMatrix();
            glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            // Bind texture
            unsigned int textureToUse = object->getTexture();
            if (textureToUse == 0)
                textureToUse = m_defaultTexture;

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureToUse);
            glUniform1i(m_textureLoc, 0);

            // Render the object
            glBindVertexArray(object->getVAO());
            glDrawElements(GL_TRIANGLES, object->getIndexCount(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }

    void ParteeRenderer::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void ParteeRenderer::present()
    {
        SwapBuffers(m_Window.getHDC());
    }

    bool ParteeRenderer::loadShaders(const std::string& vertexPath, const std::string& fragmentPath)
    {
        // Load shader source files
        std::string vertexShaderSource = loadShaderFromFile(vertexPath);
        std::string fragmentShaderSource = loadShaderFromFile(fragmentPath);

        if (vertexShaderSource.empty() || fragmentShaderSource.empty())
        {
            return false;
        }

        // Compile shaders
        unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
        unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

        if (vertexShader == 0 || fragmentShader == 0)
        {
            return false;
        }

        // Create shader program
        m_shaderProgram = createShaderProgram(vertexShader, fragmentShader);

        // Clean up individual shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return m_shaderProgram != 0;
    }

    unsigned int ParteeRenderer::createTexture(int width, int height, const void* data)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        return texture;
    }

    void ParteeRenderer::deleteTexture(unsigned int textureID)
    {
        if (textureID != 0)
        {
            glDeleteTextures(1, &textureID);
        }
    }

    void ParteeRenderer::cacheUniformLocations()
    {
        if (m_shaderProgram == 0)
            return;

        m_modelLoc = glGetUniformLocation(m_shaderProgram, "model");
        m_viewLoc = glGetUniformLocation(m_shaderProgram, "view");
        m_projectionLoc = glGetUniformLocation(m_shaderProgram, "projection");
        m_textureLoc = glGetUniformLocation(m_shaderProgram, "texture1");
    }

    unsigned int ParteeRenderer::compileShader(const std::string& source, GLenum shaderType)
    {
        unsigned int shader = glCreateShader(shaderType);
        const char* sourceCStr = source.c_str();
        glShaderSource(shader, 1, &sourceCStr, NULL);
        glCompileShader(shader);

        // Check compilation status
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::string shaderTypeStr = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
            std::string errorMsg = shaderTypeStr + " shader compilation error:\n" + std::string(infoLog);
            MessageBoxA(NULL, errorMsg.c_str(), "Shader Compilation Error", MB_OK);
            glDeleteShader(shader);
            return 0;
        }

        return shader;
    }

    unsigned int ParteeRenderer::createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
    {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Check linking status
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::string errorMsg = "Shader program linking error:\n" + std::string(infoLog);
            MessageBoxA(NULL, errorMsg.c_str(), "Shader Linking Error", MB_OK);
            glDeleteProgram(program);
            return 0;
        }

        return program;
    }

    void ParteeRenderer::createDefaultTexture()
    {
        // Create a simple gradient texture for default use
        const int width = 256;
        const int height = 256;
        unsigned char* data = new unsigned char[width * height * 4];

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int index = (y * width + x) * 4;
                float xRatio = static_cast<float>(x) / static_cast<float>(width);
                float yRatio = static_cast<float>(y) / static_cast<float>(height);

                data[index] = static_cast<unsigned char>(xRatio * 255);        // Red
                data[index + 1] = static_cast<unsigned char>(yRatio * 255);    // Green
                data[index + 2] = static_cast<unsigned char>((1.0f - xRatio) * 255); // Blue
                data[index + 3] = 255; // Alpha
            }
        }

        m_defaultTexture = createTexture(width, height, data);
        delete[] data;
    }

    std::string ParteeRenderer::loadShaderFromFile(const std::string& filePath)
    {
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::string errorMsg = "Failed to open shader file: " + filePath;
            MessageBoxA(NULL, errorMsg.c_str(), "Shader Loading Error", MB_OK);
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        return buffer.str();
    }

} // namespace Rendering
