#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "ParteeWindow.hpp"

namespace Windowing {

ParteeWindow::ParteeWindow(int width, int height) : width(width), height(height), hdc(NULL), hrc(NULL)
{
    createWindow();
    setUpOGL(hwnd);
}

ParteeWindow::~ParteeWindow()
{
    // Clean up OpenGL resources
    if (hrc)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hrc);
    }
    if (hdc && hwnd)
    {
        ReleaseDC(hwnd, hdc);
    }
}

void ParteeWindow::show() 
{
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void ParteeWindow::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update texture with new pixel data
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    // Render the pixel buffer as a textured quad
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
        glTexCoord2f(-1, 1); glVertex2i(-width, -height);
        glTexCoord2f(1, 1); glVertex2i(width, -height);
        glTexCoord2f(1, -1); glVertex2i(width, height);
        glTexCoord2f(0-1, -1); glVertex2i(-width, height);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_2D);
    
    SwapBuffers(hdc);
}

HWND ParteeWindow::getHWND() const { return hwnd; }
int ParteeWindow::getWidth() const { return width; }
int ParteeWindow::getHeight() const { return height; }

void ParteeWindow::createWindow() 
{
    const wchar_t CLASS_NAME[] = L"Partee Window Class";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Added styles for OpenGL
    wc.hbrBackground = NULL;  // No background for OpenGL

    RegisterClassW(&wc);

    // Store this pointer for the window
    hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Partee Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, GetModuleHandle(NULL), this
    );

    if (!hwnd) {
        MessageBoxW(NULL, L"Failed to create window", L"Error", MB_OK);
    }
}

void ParteeWindow::setUpOGL(HWND hwnd) {
    hdc = GetDC(hwnd);
    if (!hdc) {
        MessageBoxW(NULL, L"Failed to get device context", L"Error", MB_OK);
        return;
    }

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,                     // Color depth
        0, 0, 0, 0, 0, 0,       // Color bits (ignored)
        0,                      // No alpha buffer
        0,                      // Alpha bits (ignored)
        0,                      // No accumulation buffer
        0, 0, 0, 0,             // Accum bits (ignored)
        24,                     // Depth buffer
        8,                      // Stencil buffer
        0,                      // No auxiliary buffers
        PFD_MAIN_PLANE,         // Main layer
        0,                      // Reserved
        0, 0, 0                 // Layer masks (ignored)
    };

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!pixelFormat) {
        MessageBoxW(NULL, L"Failed to choose pixel format", L"Error", MB_OK);
        return;
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        MessageBoxW(NULL, L"Failed to set pixel format", L"Error", MB_OK);
        return;
    }

    hrc = wglCreateContext(hdc);
    if (!hrc) {
        MessageBoxW(NULL, L"Failed to create OpenGL context", L"Error", MB_OK);
        return;
    }
    if (!wglMakeCurrent(hdc, hrc)) {
        MessageBoxW(NULL, L"Failed to make context current", L"Error", MB_OK);
        return;
    }
    
    // Initialize GLEW
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        MessageBoxW(NULL, L"Failed to initialize GLEW", L"Error", MB_OK);
        return;
    }

    // Create pixel data buffer (RGBA format)
    // Create pixel data buffer (RGBA format)
    pixelData = new GLubyte[width * height * 4];

    // Initialize with some pattern (example: gradient)
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 4;
            double xRatio = (double)x / (double)width;
            double yRatio = (double)y / (double)height;

            pixelData[index] = (GLubyte)(xRatio * 255);
            pixelData[index + 1] = (GLubyte)(yRatio * 255);
            pixelData[index + 2] = (GLubyte)((1.0 - xRatio) * 255);
            pixelData[index + 3] = 255;
        }
    }

    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec2 position;

        void main() {
            gl_Position = vec4(position.xy, 0.0, 1.0);
        }
    )";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    const char* fragmentShaderSource = R"(
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

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    
    // Create texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Initialize texture with pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

    // Initialize OpenGL settings
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // Set up viewport
    glViewport(0, 0, width, height);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    
    // Switch to modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

LRESULT CALLBACK ParteeWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ParteeWindow* window = nullptr;
    
    if (uMsg == WM_CREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<ParteeWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    } else {
        window = reinterpret_cast<ParteeWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    
    switch (uMsg)
    {
    case WM_CREATE:
        return 0;
    case WM_PAINT:
    {
        if (window) {
            window->render(); 
            ValidateRect(hwnd, NULL); // Validate immediately as we're handling the drawing
            return 0;
        }
        break;
    }
    case WM_SIZE:
        if (window) {
            // Resize OpenGL viewport
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

} // namespace Windowing