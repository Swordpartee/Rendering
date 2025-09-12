#include <Windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "rendering/ParteeRenderer.hpp"
#include "rendering/ParteeWindow.hpp"

namespace Rendering
{
    ParteeRenderer::ParteeRenderer(ParteeWindow& window) : m_Window(window)
    {
        initOpenGL();
    }

    ParteeRenderer::~ParteeRenderer()
    {
        if (hrc)
        {
            wglMakeCurrent(m_Window.getHDC(), hrc); // Make context current for cleanup

            // Clean up OpenGL objects
            if (VAO != 0)
            {
                glDeleteVertexArrays(1, &VAO);
            }
            if (VBO != 0)
            {
                glDeleteBuffers(1, &VBO);
            }
            if (textureID != 0)
            {
                glDeleteTextures(1, &textureID);
            }
            if (shaderProgram != 0)
            {
                glDeleteProgram(shaderProgram);
            }
            if (pixelData != nullptr)
            {
                delete[] pixelData;
            }

            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hrc);
        }
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

        // Create pixel data buffer (RGBA format)
        pixelData = new GLubyte[m_Window.getWidth() * m_Window.getHeight() * 4];

        // Initialize with some pattern (example: gradient)
        for (int y = 0; y < m_Window.getHeight(); y++)
        {
            for (int x = 0; x < m_Window.getWidth(); x++)
            {
                int index = (y * m_Window.getWidth() + x) * 4;
                double xRatio = (double)x / (double)m_Window.getWidth();
                double yRatio = (double)y / (double)m_Window.getHeight();

                pixelData[index] = (GLubyte)(xRatio * 255);
                pixelData[index + 1] = (GLubyte)(yRatio * 255);
                pixelData[index + 2] = (GLubyte)((1.0 - xRatio) * 255);
                pixelData[index + 3] = 255;
            }
        }

        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 position;

        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )";
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        const char *fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform sampler2D texture1;

        void main() {
            vec2 texCoord = gl_FragCoord.xy / vec2(textureSize(texture1, 0));
            FragColor = texture(texture1, texCoord);
        }
    )";
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Check for shader compilation and linking errors
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            MessageBoxA(NULL, infoLog, "Shader Program Linking Error", MB_OK);
        }

        glUseProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Create texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Initialize texture with pixel data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Window.getWidth(), m_Window.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

        // Set up vertex data for a fullscreen quad
        float vertices[] = {
            // positions
            -1.0f, -1.0f,
            1.0f, -1.0f,
            1.0f, 1.0f,
            -1.0f, 1.0f};

        unsigned int indices[] = {
            0, 1, 2, // first triangle
            2, 3, 0  // second triangle
        };

        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        GLuint EBO;
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0); // Unbind VAO

        // Initialize OpenGL settings
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glDisable(GL_DEPTH_TEST); // We don't need depth testing for a 2D texture

        // Set up viewport
        glViewport(0, 0, m_Window.getWidth(), m_Window.getHeight());
    }

    void ParteeRenderer::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // Update texture with new pixel data
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Window.getWidth(), m_Window.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

        // Use our shader program
        glUseProgram(shaderProgram);

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        // Render the fullscreen quad using modern OpenGL
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SwapBuffers(m_Window.getHDC());
    }

} // namespace Rendering
